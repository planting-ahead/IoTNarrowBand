# Download the helper library from https://www.twilio.com/docs/python/install
from twilio.rest import Client


# Your Account Sid and Auth Token from twilio.com/console
account_sid = 'AC9510211370ca5e7f76f50b1262cf1881'
auth_token = 'your_auth_token'
client = Client(account_sid, auth_token)

message = client.messages.create(
                              from_='+15017122661',
                              body='body',
                              to='+15558675310'
                          )

print(message.sid)
