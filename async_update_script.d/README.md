# BASH Trello API

To use these commands... 

- you need to get your `TRELLO_API_KEY` and `TRELLO_API_TOKEN` from the Trello website, so you can set them as environment variables in `trello_auth_creds`. 
- you must run the ./trello_auth in the shell before any other commands (you only have to run it once though).
- scripts that will get used often, like async_updates and update_brand, have named arguments, but most of scripts don't. `cat` to see the order of arguments, as the variables are very informative
