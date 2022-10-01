/**
 * harbour-commbank - Sailfish OS Version
 * Copyright © 2022 Andreas Wüst (andreas.wuest.freelancer@gmail.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
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
const char URL_BROKERAGE_DEPOTS[] =
    "https://api.comdirect.de/api/brokerage/clients/user/v3/depots";
const char URL_ACCOUNT_TRANSACTIONS[] =
    "https://api.comdirect.de/api/banking/v1/accounts/%1/"  // accountId
    "transactions?transactionState=BOOKED&paging-first=%2"; // startIndex
const char URL_BROKERAGE_POSITIONS[] =
    "https://api.comdirect.de/api/brokerage/v3/depots/%1/positions?"
    "with-attr=instrument&paging-first=%2"; // depotId, startIndex

#endif // CONSTANTS_H
