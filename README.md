# CoC
CoC - Coffee over Coffee

This is the implementation of the CoC project.

# CoC - In-depth view
CoC project is aimed for being a fun and useless build.
The main idea behind this project is to pass data describing
how a user would like to drink his coffee over an unusual medium - coffee beans.

In one end the user would find a screen with UI (written in coffeescript) where
he should choose his preferred way of drinking his coffee,
then his choice will be encoded to binary form.
After we have his choice the machine shall shoot coffee beans to the other section
of it, the shooting of beans should implement some sort of half-duplex UART protocol
so the other section of the machine could parse the coffee beans as binary data...
Finally after the beans had been shot in the air the decoder section of the machine
would parse the given user choice and use a coffee machine to make his coffee
just as the user desired.
