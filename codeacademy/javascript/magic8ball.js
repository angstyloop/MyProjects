// A toy JS example: a magic eight ball with random
// responses.

let userName = "Sean";
let userQuestion = "Will I graduate?";
let eightBall = "";
const msg1 = "Hello!";
const msg2 = `Hello, ${userName}!`;
let greeting = "";
userName ? greeting=msg2 : greeting=msg1;
console.log(greeting);
console.log(userQuestion);
let randomNumber = Math.floor(Math.random()*8);
switch (randomNumber) {
  case 0:
    eightBall = 'It is certain.';
    break;
  case 1:
    eightBall = "It is decidedly so."
    break;
  case 2:
    eightBall = "Reply hazy try again."
    break;
  case 3:
    eightBall = "Cannot predict now."
  	break;
  case 4:
    eightBall = "Do not count on it."
    break;
  case 5:
    eightBall = "Outlook not so good."
    break;
  case 6:
    eightBall = "Signs point to yes."
    break;
  case 7:
    eightBall = "My sources say no."
    break;
};
console.log(eightBall);

