## kuma

### What is this?

A programming language I made in a day. I did not follow good code practices with this, but it works. It might also be considered Turing complete since it can emulate Conway's game of life, which is Turing complete.

### How does it work?

Lexical analysis of input creates tokens. Tokens are fed into a parser. Parser does the stuff. Conditional statements and loops are secretly functions. The syntax is like reverse Polish notation, but not consistently. It's weird. Check out the examples.

### Why did you make this?

I wanted to see what reverse Polish JavaScript would look like.

### How do I use this?

Please don't. But if you insist, you can run `make` and use `./dist/kuma ./examples/coolConway.kma`. There's also a really terrible syntax highlighting schema in the `kuma` folder. If you want to write your own code, I honestly don't remember how half of this works anymore. Be extremely familiar with reverse Polish notation. If there's bugs, use the `$` operator to forcefully access the value of variable, as variables are often used referentially. Remember that everything works using a stack, and that is how functions return values.
