// Sean Allen (github.com/angstyloop)

// Teachers of the blind and visually impaired often
// design braille lesson material based on the
// contractions the student has learned so far.
// This is actually quite impressive, for there are
//  137+ braille contractions, not to mention all the
// tricky exceptions that must be memorized.

// The goal of this braille_word_generator project
// is to take a subset of the contractions, selected
// by the user and given as input, and then generate
// a word list of sufficient length to tell a story
// to the student at their level of comprehension
// and with only words from the list.

// The easiest interface for a teacher is probably
// a website, where they can use a combination of
// search bar, textarea,  and selection lists to
// prune their set of contractions. We could also
// add user authentication later, so users could
// save their contractions for later. Text-file
// upload might also be useful.

// Our approach to this problem is to create 
//   equivalence classes for words in the
//   dictionary with the same contractions.
//   We can label each of these classes with
//   run-length encoded strings.
//
//   e.g. a2bc3  , where a, b, and c are
//   contractions.
//
//  We can then map these string labels
//  to lists of words in their equivalence
//  classes. This should give amortized 
//  constant time lookup for individual
//  classes.
//
//  Thus we need a function
//  parseDictionary() that takes a 
//  dictionary of words and uses it to 
//  create the map.

