# function-fsm
A library that simplifies the creation of finite state machines and streamlines the process of turning state diagrams into code. It encourages objects to have ownership of their state by allowing finite state machines to be held by other objects.

This library works in any C++11 supported environment, including Arduino compatible microcontrollers that have implementations of std::chrono, std::function and std::vector (eg. ESP32).

Examples of use are included in the library, they compile in the Arduino environment but show how the library can be used in general.

# Contribution
Feel free to submit pull-requests for any features or improvements you'd like to develop.

# Credit
Major credit to Jon Black's [arduino-fsm](https://github.com/jonblack/arduino-fsm) - an arduino library for implementing finite state machines.

function-fsm was developed to expand upon the functionality of this library (allowing FSM's to be used within other classes, allowing the library to be used outside of the Arduino environment, and providing other useful features too!). While a lot of the code has been rewritten, function-fsm leans heavily on the structure of arduino-fsm. Many thanks to [Jon Black](https://github.com/jonblack) for having provided such a useful arduino library

Also, the top answer to my [Stack Overflow question](https://stackoverflow.com/questions/55513790/understanding-error-with-taking-address-of-non-static-member-to-form-pointer-to) that sparked this, thank you.
