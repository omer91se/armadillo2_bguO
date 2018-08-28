# Speech overview

The speech bundle supports both voice recognition and text to speech.

##bing2.py

A module used to send a speech wav file to bing services and retrieve a string of the speech.
Please set "BING_KEY" in ~/.bashrc before use.

## objects_detection.py

A module used to send GET/POST requests to the server (code located in /Vision/nn_object_detection/nn_server.py)

parse_query() sends a GET request receiving a query string as a parameter. It returns the reconized subject (noun) and the appropriate maskrcnn label.

processRequest() sends a POST request receving a raw RGB image as a parameter. It returns the recognized objects labels and their bounding-boxes in the given image.

## scene_description.py

processRequest() send a POST request receving a raw RGB image as a parameter. It returns an image description (text/string).
It uses Microsoft Vision service, so setting MICROSOFT_VISION_KEY in ~/.bashrc is a prerequisite.

## tts.py

tts() receives a string and says it out loud (text-to-speech) using Bing service. Set BING_KEY in ~/.bashrc before use.
ding() plays a ding sound, indicating it's time to talk to the microphone.

## voice_recognition.py

This module waits for speech from the user's microphone (wait in silence for the "ding" sound before start talking) and publish the recognized text to the "speech_text" ROS topic.
