#!/usr/bin/env python
# -*- coding: utf-8 -*-

# ----- IMPORT LIBRARIES -----

# *** TWITTER ***
# credentials from https://apps.twitter.com/
import tweepy
from tweepy.streaming import StreamListener
from tweepy import OAuthHandler
from tweepy import Stream

import csv
import pandas as pd
import json
# from json_utils import has_attribute

# *** WATSON ***
import watson_developer_cloud
from watson_developer_cloud import NaturalLanguageUnderstandingV1
from watson_developer_cloud import NaturalLanguageUnderstandingV1 as NaturalLanguageUnderstanding
from watson_developer_cloud.natural_language_understanding_v1 import Features, EntitiesOptions, KeywordsOptions

# *** SOUND ***
from playsound import playsound

# *** SERIAL COMMUNICATION ***
import serial
import time

# ----- MAIN CODE -----

# *** TWITTER CREDENTIALS ***
consumer_key = "nNgDoEhcjFHLh9E27dFe9cf04"
consumer_secret = "y6nzVaPzsW7AJ01EPUiNhTvsugv9LOOzYELxUtX3d8z20DiWid"
access_token = "2370070339-HN4OrLJenRnXejj4EMCvaszHYTGg9pBsz7B8btI"
access_token_secret = "aEyGT4emvGb78ENXlcmyr5EYD6JOnjTUkxwzbRF8uIXyG"

# def findEmotion():


def tweetEmotion(tweet):
    # *** WATSON ANALYSIS ***
    #returns emotion: joy, anger, disgust, sadness, fear

    # ----- WATSON CLOUD LOGIN -----
    natural_language_understanding = NaturalLanguageUnderstandingV1(
      username="b1e09b94-9bf4-4b0f-8e93-046b5b6c4123",
      password="YpQgaNCOa8Ns",
      version="2017-02-27")

    try:
        response = natural_language_understanding.analyze(
          text=tweet,
          features=Features(entities=EntitiesOptions(
                                  emotion=True, sentiment=True,limit=1),
                           keywords=KeywordsOptions(
                                  emotion=True, sentiment=True,limit=1
                                            ))
        )

    except watson_developer_cloud.watson_service.WatsonApiException as e:
        response = []

    # print "response", response

    json_data = json.dumps(response, indent=2)          #loads into objects
    json_data = json.loads(json_data)                   #converts into dictionary

    # print "jason_data", json_data

    if len(json_data) > 0:
        # check where emotions located: keywords or entities
        # print len(json_data["keywords"])
        # print len(json_data["entities"])

        #check if emotion data exists
        if (len(json_data["keywords"]) > 0 and "emotion" in json_data["keywords"][0]):
            result = json_data["keywords"][0]["emotion"]
            print result
        elif (len(json_data["entities"]) > 0 and "emotion" in json_data["entities"][0]):
            result = json_data["entities"][0]["emotion"]
            print result
        else:
            result = "undef"
            print result
            # return result
    else:
        result = "undef"
        # print result
        # return result

    if result == "undef":
        return "undef"
    else:
        maxEmotion = max(result, key=result.get)

    print maxEmotion
    # sound(maxEmotion)

    # return maxEmotion

def sound(mainEmotion):
    # anger, disgust, fear, joy, sadness, undefined
    file = open("twitterradio/soundFile.txt", "w")
    file.write(mainEmotion)
    file.close()
    # if mainEmotion == "joy":
    #     print "hello joy sound"
    #     playsound('andrea-devore-assign4.mp3')

class StdOutListener(StreamListener):

    def on_data(self, data):
        # print "data"
        d = json.loads(data)
        # print d
        if "text" in d:
            tweet = d["text"]
            print tweet
            mainEmotion = tweetEmotion(tweet)
            sound(mainEmotion)
        return True

    def on_error(self, status):
        print "status"
        print status

if __name__ == '__main__':
    #This handles Twitter authetification and the connection to Twitter Streaming API
    l = StdOutListener()
    auth = OAuthHandler(consumer_key, consumer_secret)
    auth.set_access_token(access_token, access_token_secret)
    stream = Stream(auth, l)

    #This line filter Twitter Streams to capture data by the keywords: 'python', 'javascript', 'ruby'
    stream.filter(languages=["en"], track=['#twitterradio'])
