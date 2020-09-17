# Async Updates & BASH Trello API

## async_updates

This script asynchronously updates all brand clusters defined in brand_trello_cards and updates Trello, by moving card to new release's list and removing "Approved Ready to Update" label on success, or by leaving card on the old release's list and adding a "Failed to Update" label on failure, and then logging the result to S3 in each case. The script waits for each update to finish, and cleans up local temporary logs before exiting.

## To use these BASH Trello API commands... 

- you need to get your `TRELLO_API_KEY` and `TRELLO_API_TOKEN` from the Trello website, so you can set them as environment variables in `trello_auth_creds`. right now they are set to me @sean, but ideally we have a Trello user just for that ... although we could save ~ $17 / month or something by not making a new user.

- you must run the ./trello_auth in the shell before any other commands (you only have to run it once though).

- scripts that will get used often, like async_updates and update_brand, have named arguments, but most of scripts don't. use the `cat`, `less`, or `vim` (or `emacs` you handsome devil) commands to see the order of arguments, as the variable names make it clear.

# helper scripts

```
# use Trello to get the release name for a brand
./which_release edu

# list all the brand shortnames (fox, edu, etc.)
./list_brands
```

examples

```
# get board
boardId=$(./get_board_id TestBoard12345)

# get or create list
listId=$(./get_or_create_list_id $boardId TheListOfAFunkyBrewster)

# get cards on board
./get_cards_on_board $boardId | jq

# get cards on list
./get_cards_on_list $listId | jq

# get or create card
cardId=$(./get_or_create_card $listId TheCardOfAFunkyBrewster)

# get or create label
labelId=$(./get_or_create_label $boardId TheLabelOfAFunkyBrewster green)

# add/remove label
./add_label_to_card $cardId $labelId
./remove_label_from_card $cardId $labelId

# move card to other list
otherListId=$(get_or_create_list_id $boardId TheListOfAnotherFunkyBrewster)
./move_card $cardId $boardId $otherListId

```
