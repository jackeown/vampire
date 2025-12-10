/*
 * This file is part of the source code of the software program
 * Vampire. It is protected by applicable
 * copyright laws.
 *
 * This source code is distributed under the licence found here
 * https://vprover.github.io/license.html
 * and in the source directory
 */

#include "Indexing/TermIndex.hpp"
#include "Saturation/SaturationAlgorithm.hpp"
#include "Test/UnitTesting.hpp"
#include "Test/SyntaxSugar.hpp"
#include "Test/GenerationTester.hpp"

#include "Inferences/Superposition.hpp"

using namespace Test;

/**
 * NECESSARY: We need to tell the tester which syntax sugar to import for creating terms & clauses.
 * See Test/SyntaxSugar.hpp for which kinds of syntax sugar are available
 */
#define MY_SYNTAX_SUGAR                                                                                       \
  DECL_DEFAULT_VARS                                                                                           \
  DECL_SORT(s)                                                                                                \
  DECL_FUNC(f, {s, s}, s)                                                                                     \
  DECL_FUNC(g, {s}, s)                                                                                        \
  DECL_CONST(a, s)                                                                                            \
  DECL_PRED (p, {s})                                                                                          \
  DECL_PRED (q, {s})                                                                                          \

Generation::TestIndices superpositionIndices()
{
  return {
    [](const SaturationAlgorithm& alg){
      return new Indexing::SuperpositionLHSIndex(new Indexing::TermSubstitutionTree<TermLiteralClause>(), alg.getOrdering(), alg.getOptions()); },
    [](const SaturationAlgorithm& alg){
      return new Indexing::SuperpositionSubtermIndex(new Indexing::TermSubstitutionTree<TermLiteralClause>(), alg.getOrdering()); },
  };
}

REGISTER_GEN_TESTER(Generation::GenerationTester<Inferences::Superposition>(Superposition()))

// no superposition with negative equations
TEST_GENERATION(test_01,
    Generation::SymmetricTest()
      .indices(superpositionIndices())
      .inputs({
        clause({ selected(f(x,y) != x) }),
        clause({ selected(f(x,y) != y) })
      })
      .expected(none())
    )

// self superposition with equation
TEST_GENERATION(test_02,
    Generation::SymmetricTest()
      .indices(superpositionIndices())
      .inputs({ clause({ selected(f(f(x,y),z) == x) }) })
      .expected({ clause({ f(x,y) == f(x,z) }) })
    )

// superposition only into bigger side of the equation
// superposition only into selected literals
// superposition resulting in trivial equality
// simultaneous superposition
// non-simultaneous superposition
// superposition into variables
// superposition maximality aftercheck