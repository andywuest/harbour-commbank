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

private:
    QString accessTokenLogin; // accessToken when logging in
    QString refreshTokenLogin; // refreshToken when logging in
    QString accessToken;   // accessToken after complete 2FA login
    QString refreshToken;  // refreshToken after complete 2FA login
    QString sessionId;
    QString requestId;
    QString identifier;
    QString challengeId;
    QString challengeType;

    QString createRequestId();
    QString createSessionId();

};

#endif // SESSIONCONTEXT_H
