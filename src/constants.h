#ifndef CONSTANTS_H
#define CONSTANTS_H

const char MIME_TYPE_JSON[] = "application/json";
const char MIME_TYPE_WWW_FORM_URL_ENCODED[] =
    "application/x-www-form-urlencoded";
const char USER_AGENT[] = "Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:73.0) "
                          "Gecko/20100101 Firefox/73.0";

const char URL_OAUTH_TOKEN[] = "https://api.comdirect.de/oauth/token";
const char URL_SESSIONS[] =
    "https://api.comdirect.de/api/session/clients/user/v1/sessions";
const char URL_CREATE_SESSION_TAN[] =
    "https://api.comdirect.de/api/session/clients/user/v1/sessions/%1/validate";
const char URL_ACTIVATE_SESSION_TAN[] =
    "https://api.comdirect.de/api/session/clients/"
    "user/v1/sessions/%1"; // sessionId
const char URL_ACCOUNT_BALANCES[] =
    "https://api.comdirect.de/api/banking/clients/user/v2/accounts/balances";

#endif // CONSTANTS_H
