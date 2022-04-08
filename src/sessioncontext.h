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
#ifndef SESSIONCONTEXT_H
#define SESSIONCONTEXT_H

#include <QObject>

class SessionContext {

public:
  SessionContext() = default;

  QString getAccessTokenLogin();
  void setAccessTokenLogin(const QString &accessTokenLogin);

  QString getAccessToken();
  void setAccessToken(const QString &accessToken);

  QString getRefreshTokenLogin();
  void setRefreshTokenLogin(const QString &refreshTokenLogin);

  QString getRefreshToken();
  void setRefreshToken(const QString &refreshToken);

  QString createRequestInfoString();

  QString getSessionId();
  QString getRequestId();

private:
  QString accessTokenLogin;  // accessToken when logging in
  QString refreshTokenLogin; // refreshToken when logging in
  QString accessToken;       // accessToken after complete 2FA login
  QString refreshToken;      // refreshToken after complete 2FA login
  QString sessionId;
  QString requestId;
  QString identifier;
  QString challengeId;
  QString challengeType;

  QString createRequestId();
  QString createSessionId();
};

#endif // SESSIONCONTEXT_H
