//===--- State.cpp - State chain for the VM and AST Walker ------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "State.h"
#include "Frame.h"
#include "Program.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/CXXInheritance.h"
#include "clang/AST/OptionalDiagnostic.h"

using namespace clang;
using namespace clang::interp;

State::~State() {}

OptionalDiagnostic State::FFDiag(SourceLocation Loc, diag::kind DiagId,
                                 unsigned ExtraNotes) {
  return diag(Loc, DiagId, ExtraNotes, InterpDiagKind::FF);
}

OptionalDiagnostic State::FFDiag(const Expr *E, diag::kind DiagId,
                                 unsigned ExtraNotes) {
  return FFDiag(E->getExprLoc(), DiagId, ExtraNotes);
}

OptionalDiagnostic State::FFDiag(const SourceInfo &SI, diag::kind DiagId,
                                 unsigned ExtraNotes) {
  return FFDiag(SI.getLoc(), DiagId, ExtraNotes);
}

OptionalDiagnostic State::CCEDiag(SourceLocation Loc, diag::kind DiagId,
                                  unsigned ExtraNotes) {
  return diag(Loc, DiagId, ExtraNotes, InterpDiagKind::CCE);
}

OptionalDiagnostic State::CCEDiag(const Expr *E, diag::kind DiagId,
                                  unsigned ExtraNotes) {
  return CCEDiag(E->getExprLoc(), DiagId, ExtraNotes);
}

OptionalDiagnostic State::CCEDiag(const SourceInfo &SI, diag::kind DiagId,
                                  unsigned ExtraNotes) {
  return CCEDiag(SI.getLoc(), DiagId, ExtraNotes);
}

OptionalDiagnostic State::PCFDiag(SourceLocation Loc, diag::kind DiagId,
                                  unsigned ExtraNotes) {
    return diag(Loc, DiagId, ExtraNotes,
		partialConstantEvaluation() ? InterpDiagKind::PCF : InterpDiagKind::CCE);
}

OptionalDiagnostic State::PCFDiag(const Expr *E, diag::kind DiagId,
                                  unsigned ExtraNotes) {
  return PCFDiag(E->getExprLoc(), DiagId, ExtraNotes);
}

OptionalDiagnostic State::PCFDiag(const SourceInfo &SI, diag::kind DiagId,
                                  unsigned ExtraNotes) {
  return PCFDiag(SI.getLoc(), DiagId, ExtraNotes);
}

OptionalDiagnostic State::Note(SourceLocation Loc, diag::kind DiagId) {
  if (!hasActiveDiagnostic())
    return OptionalDiagnostic();
  return OptionalDiagnostic(&addDiag(Loc, DiagId));
}

void State::addNotes(ArrayRef<PartialDiagnosticAt> Diags) {
  if (hasActiveDiagnostic()) {
    getEvalStatus().Diag->insert(getEvalStatus().Diag->end(), Diags.begin(),
                                 Diags.end());
  }
}

DiagnosticBuilder State::report(SourceLocation Loc, diag::kind DiagId) {
  return getCtx().getDiagnostics().Report(Loc, DiagId);
}

/// Add a diagnostic to the diagnostics list.
PartialDiagnostic &State::addDiag(SourceLocation Loc, diag::kind DiagId) {
  PartialDiagnostic PD(DiagId, getCtx().getDiagAllocator());
  getEvalStatus().Diag->push_back(std::make_pair(Loc, PD));
  return getEvalStatus().Diag->back().second;
}

OptionalDiagnostic State::diag(SourceLocation Loc, diag::kind DiagId,
                               unsigned ExtraNotes, InterpDiagKind InterpDiag) {
  Expr::EvalStatus &EvalStatus = getEvalStatus();
  if (!EvalStatus.Diag) {
    setActiveDiagnostic(false);
    return OptionalDiagnostic();
  }
  
  if (hasPriorDiagnostic()) {
    return OptionalDiagnostic();
  }

  unsigned CallStackNotes = getCallStackDepth() - 1;
  unsigned Limit = getCtx().getDiagnostics().getConstexprBacktraceLimit();
  if (Limit)
    CallStackNotes = std::min(CallStackNotes, Limit + 1);
  if (checkingPotentialConstantExpression())
    CallStackNotes = 0;

  setActiveDiagnostic(true);
  setFoldFailureDiagnostic(InterpDiag == InterpDiagKind::FF);
  setCCEDiagnostic(InterpDiag == InterpDiagKind::CCE);
  if (InterpDiag != InterpDiagKind::PCF)
    EvalStatus.Diag->clear();
  EvalStatus.Diag->reserve(1 + ExtraNotes + CallStackNotes);
  addDiag(Loc, DiagId);
  if (!checkingPotentialConstantExpression()) {
    addCallStack(Limit);
  }
  return OptionalDiagnostic(&EvalStatus.Diag->back().second);
}

const LangOptions &State::getLangOpts() const { return getCtx().getLangOpts(); }

void State::addCallStack(unsigned Limit) {
  // Determine which calls to skip, if any.
  unsigned ActiveCalls = getCallStackDepth() - 1;
  unsigned SkipStart = ActiveCalls, SkipEnd = SkipStart;
  if (Limit && Limit < ActiveCalls) {
    SkipStart = Limit / 2 + Limit % 2;
    SkipEnd = ActiveCalls - Limit / 2;
  }

  // Walk the call stack and add the diagnostics.
  unsigned CallIdx = 0;
  Frame *Top = getCurrentFrame();
  const Frame *Bottom = getBottomFrame();
  for (Frame *F = Top; F != Bottom; F = F->getCaller(), ++CallIdx) {
    SourceLocation CallLocation = F->getCallLocation();

    // Skip this call?
    if (CallIdx >= SkipStart && CallIdx < SkipEnd) {
      if (CallIdx == SkipStart) {
        // Note that we're skipping calls.
        addDiag(CallLocation, diag::note_constexpr_calls_suppressed)
            << unsigned(ActiveCalls - Limit);
      }
      continue;
    }

    // Use a different note for an inheriting constructor, because from the
    // user's perspective it's not really a function at all.
    if (auto *CD = dyn_cast_or_null<CXXConstructorDecl>(F->getCallee())) {
      if (CD->isInheritingConstructor()) {
        addDiag(CallLocation, diag::note_constexpr_inherited_ctor_call_here)
            << CD->getParent();
        continue;
      }
    }

    SmallString<128> Buffer;
    llvm::raw_svector_ostream Out(Buffer);
    F->describe(Out);
    addDiag(CallLocation, diag::note_constexpr_call_here) << Out.str();
  }
}
