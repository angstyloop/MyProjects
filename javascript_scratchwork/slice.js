// how does slice work? turns out the syntax is nice.

const s = "bees"

console.log("\n")
console.log("Look at the source and the output below to understand slice.")

console.log("heads")
console.log("-----")
console.log(`\'${s.slice(0,0)}\'`)
console.log(`\'${s.slice(0,1)}\'`)
console.log(`\'${s.slice(0,2)}\'`)
console.log(`\'${s.slice(0,3)}\'`)
console.log(`\'${s.slice(0,4)}\'`)

console.log('\n')

console.log("tails")
console.log("-----")
console.log(`\'${s.slice(0)}\'`)
console.log(`\'${s.slice(1)}\'`)
console.log(`\'${s.slice(2)}\'`)
console.log(`\'${s.slice(3)}\'`)
console.log(`\'${s.slice(4)}\'`)

console.log('\n')
console.log("Be sure to check out the useful identities asserted in the source!")

// useful identities
const L = s.length
for (let i=0; i<L; i++) {
  console.assert( s.slice(0,i) + s.slice(i) )
  console.assert( s === s.slice(0,i) + s.charAt(i) + s.slice(i+1) )
  for (let j=0; j<i; j++) {
    console.assert( s === s.slice(0, j) + s.slice(j, i) + s.slice(i) )
    console.assert( s === s.slice(0, j) + s.charAt(j) + s.slice(j+1, i) + s.charAt(i) + s.slice(i+1) )
  }
}

