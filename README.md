
# Commbank
A native comdirect banking client for Sailfish OS.

## Author
Andreas Wüst [andreas.wuest.freelancer@gmail.com](mailto:andreas.wuest.freelancer@gmail.com)

## License
Licensed under GNU GPLv3

## Preconditions for usage

You need to be customer of comdirect. In order to be able to use the app you have to activate
the usage of the REST Api. You can do this directly in the comdirect Online-Banking via 
*Persönlicher Bereich -> Verwaltung -> Entwickerzugang*.

You will be provided a *client_id* and and a *client_secrect* which you need in order to log in
additionally to the already known Zugangskennung and PIN.

## Implementation

### What does work

Currently the following features are implemented.

- Login with two factor authentication
    - SMS TAN (should work, but not tested. For each login you will get a SMS which can become expensive)
    - Photo TAN with separate hardware device (not yet implemented, need a sample of the provided image data, easy to implement)
    - Push TAN with Mobile App for Andriod/IOS. Works with the Android App - IOS most likely also works.
- Displaying the list of accounts with balances
- Selecting an account to view the payment transactions (in work)
- Support for multiple accounts
- Storing and Loading of credentials (client_id, client_secret, Zugangskennung) - PIN is not stored.

### What does not work - what is missing

- Token Refresh (after 5 Minutes token becomes invalid and no further requests are possible)
- Show portfolios and portfolio items.
- No icon so far
- Proper translation
- UI needs to be improved
- No transactions - transfering money or buying / selling securites is not on the roadmap (but i am open for pull requests)


## Scripts

### License Information Generation

```
mvn license:format
mvn license:remove
```
