# run_object_rec.py

Listens to the microphone or gets input from the keyboard, asking the user: "Hi there! How can I help?"
The user says a sentence/single word containing a noun.

The code then detects the object in the image (robot's camera), asking disambiguation follow-up question if needed: "Do you mean the cup on the left or the cup on the right?" and returns its coordinates: x, y (center of the bounding-box in meters), z (distance to the center in meters), w and h (width and height in pixels).
