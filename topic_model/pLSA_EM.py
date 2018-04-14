#!/bin/python
# -*- coding: UTF-8 -*-
import random
import json

import numpy as np


def count_WordDoc(doc_list, wordsid_file):
    """
    统计频率
    :param doc_list: 文档列表  [[word1 word2] [word1 word2]]
    :return: doc_word {dco1{word1:count}},  words [word1 word2]
    """
    words = set()
    doc_len = len(doc_list)
    doc_word = dict()
    for i in range(doc_len):
        doc_word[i] = dict()
        for word in doc_list[i]:
            words.add(word)
            if word not in doc_word[i].keys():
                doc_word[i][word] = 1
            else:
                doc_word[i][word] = doc_word[i][word] + 1
    words = list(words)
    with open(wordsid_file, 'w') as fopen:
        json.dump(dict(zip(words, range(len(words)))), fopen)
    return doc_word, words


def load_word_id(wordsid_file):
    with open(wordsid_file) as fopen:
        word_id = json.load(fopen)
    return word_id


def save_model(doc_topic, topic_words):
    """
    
    :param doc_topic: 
    :param topic_words: 
    :return: 
    """
    np.save("doc2topic.npy", doc_topic)
    np.save("topic2words.npy", topic_words)


def load_model(doc_topic_file="doc2topic.npy", topic_words_file="topic2words.npy"):
    """
    
    :param doc_topic_file: 
    :param topic_words_file: 
    :return: 
    """
    doc_topic = np.load(doc_topic_file)
    topic_words = np.load(topic_words_file)
    return doc_topic, topic_words


def plsa_em(doc_word, words, k, iter_num):
    """
    pLSA的EM解法
    :param doc_word: doc_word {dco1{word1:count}}
    :param words: words [word1 word2]
    :param k: 主题数
    :return: doc_topic, topic_words
    """
    doc_topic, topic_words, doc_k_words, doc_word_count = init_param(doc_word, words, k)
    for i in range(iter_num):
        e_step(doc_topic, topic_words, doc_k_words)
        m_step(doc_word_count, doc_topic, topic_words, doc_k_words)
    save_model(doc_topic, topic_words)
    return doc_topic, topic_words


def e_step(doc_topic, topic_words, doc_k_words):
    """
    EM算法
    :param doc_topic: 
    :param topic_words: 
    :param doc_k_words: 
    :return: 
    """
    dim = doc_k_words.shape
    for i in range(dim[0]):
        for j in range(dim[1]):
            sum_1 = sum(doc_topic[i]*topic_words[:, j])
            for k in range(dim[2]):
                doc_k_words[i][j][k] = doc_topic[i][k]*topic_words[k][j]/sum_1 if sum_1 !=0 else 0


def m_step(doc_word, doc_topic, topic_words, doc_k_words):
    """
    EM算法
    :param doc_word: 
    :param doc_topic: 
    :param topic_words: 
    :param doc_k_words: 
    :return: 
    """
    dim = doc_k_words.shape
    for i in range(dim[0]):
        for k in range(dim[2]):
            doc_topic[i][k] = sum(doc_word[i]*doc_k_words[i, :, k])
        doc_topic[i] = doc_topic[i]/sum(doc_topic[i])
    for k in range(dim[2]):
        for j in range(dim[1]):
            topic_words[k][j] = sum(doc_word[:, j]*doc_k_words[:, j, k])
        topic_words[k, :] = topic_words[k, :]/sum(topic_words[k, :])


def init_param(doc_word, words, k):
    """
    初始化
    :param doc_len: 
    :param word_len: 
    :param k: 
    :return: 
    """
    doc_len = len(doc_word)
    word_len = len(words)
    doc_topic = np.zeros((doc_len, k))
    topic_words = np.zeros((k, word_len))
    doc_k_words = np.zeros((doc_len, word_len, k))
    doc_word_count = np.zeros((doc_len, word_len))
    for i in range(doc_len):
        for j in range(word_len):
            doc_word_count[i][j] = doc_word[i][words[j]] if words[j] in doc_word[i].keys() else 0
    random.seed(0)
    for i in range(doc_len):
        for j in range(k):
            doc_topic[i][j] = random.random()
        doc_topic[i] = doc_topic[i]/sum(doc_topic[i])
    for i in range(k):
        for j in range(word_len):
            topic_words[i][j] = random.random()
        topic_words[i] = topic_words[i]/sum(topic_words[i])
    return doc_topic, topic_words, doc_k_words, doc_word_count


def predict(topic_words, words, words_id):
    w_id = list()
    for w in words:
        w_id.append(words_id[w] if w in words_id.keys() else 0)
    dim = topic_words.shape[0]
    docn_topic = np.zeros(dim)
    for i in w_id:
        docn_topic = docn_topic + topic_words[:, i]
    docn_topic = docn_topic / sum(docn_topic)
    return docn_topic


if __name__ == "__main__":
    t = 1
    if t == 0:
        doc_list = [[1,2,2,3,3], [2,2,3,3,3,4], [5,6,6,6,7,7,7,7], [66,6,6,6,7,6,7,7,7,8,9]]
        wordsid_file = "word_id"
        doc_word, words = count_WordDoc(doc_list, wordsid_file)
        doc_topic, topic_words = plsa_em(doc_word, words, 2, 20)

    else:
        # test
        wordsid_file = "word_id"
        doc_topic, topic_words = load_model()
        words = [6,6,7,7,1,4]
        words_id = load_word_id(wordsid_file)
        print predict(topic_words, words, words_id)
    # print doc_word
    # print words
    # print doc_topic
    # print topic_words
