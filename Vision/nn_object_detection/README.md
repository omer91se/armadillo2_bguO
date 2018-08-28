# Vision and Language Parser Server

Usage

python3 server.py [\<port\>] # port is optional

## Web Services

### parse_query

GET Request:

\<server-ip-address\>:\<port\>/parse_query?query=show me the cup

returns {'subject': subject, 'label': label} where subject is the query's noun and label is the appropriate MaskRCNN label that best fits the subject.

### maskrcnn

POST request

\<server-ip-address\>:\<port\>/maskrcnn

The data sent to this service is a raw image RGB data.

The response is a list of recongnized objects and their bounding boxes coordinates
