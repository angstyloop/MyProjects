//When the first argument to String::replace is a VALUE, rather than a REGEXP, only the 
// first match is replaced. To substitute globally, use a regexp and the g modifer. 

const str = " a b c   d e f "
const compactStr0 = str.replace(' ', '')
const compactStr1 = str.replace(/ /, '')
const compactStr2 = str.replace(/\s/g, '')
const compactStr3 = str.replace(/ /g, '')
Array.of(compactStr0, compactStr1, compactStr2, compactStr3).forEach(it => console.log(it))
