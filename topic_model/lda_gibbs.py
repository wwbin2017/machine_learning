#!/bin/python
# -*- coding: UTF-8 -*-
import random
import json

import numpy as np


def count_WordDoc(doc_list, wordsid_file):
    """
    统计频率
    :param doc_list: 文档列表  [[word1 word2] [word1 word2]]
    :return: doc_word [[word1_id word2_id] [word1_id word2_id]],  words {word:id}
    """
    words = set()
    doc_len = len(doc_list)
    for i in range(doc_len):
        for word in doc_list[i]:
            words.add(word)
    words = list(words)
    words_id = dict(zip(words, range(len(words))))
    with open(wordsid_file, 'w') as fopen:
        json.dump(words_id, fopen)
    doc_word = list()
    for i in range(len(doc_list)):
        doc_word.append(list())
        for w in doc_list[i]:
            doc_word[i].append(words_id[w])
    return doc_word, words_id


def lda_gibbs(doc_word, words_id, k, alpha, beta, iter_num):
    """
    LDA模型
    :param doc_word: [[word_id word_id] [word_id word_id]] 
    :param words_id: {word:id}
    :param k:  主题数
    :param alpha: 文档主题参数服从dirichlet分布的参数
    :param beta:    主题词参数服从dirichlet分布的参数
    :param iter_num: 迭代次数
    :return: theta 文档主题分布, phi 主题词分布
    """
    doc_topic_count, topic_words_count, doc_word_count = init_param_lda(doc_word, words_id, k)
    for i in range(iter_num):
        gibbs_sampling(doc_topic_count, topic_words_count, doc_word, doc_word_count, k, alpha, beta)
    theta = calc_theta(doc_topic_count, alpha)
    phi = calc_phi(topic_words_count, beta)
    save_model(theta, phi)
    return theta, phi


def calc_theta(doc_topic_count, alpha):
    """
    计算文档主题分布
    :param doc_topic_count: np.array dim=[document, topic] 
    :param alpha: 
    :return: theta np.array dim=[document, topic] 
    """
    dim = doc_topic_count.shape
    theta = np.zeros(dim)
    for i in range(dim[0]):
        sum_i = sum(doc_topic_count[i, :]) + dim[1]*alpha
        for j in range(dim[1]):
            theta[i, j] = (doc_topic_count[i, j] + alpha)*1.0/sum_i
    return theta


def calc_phi(topic_words_count, beta):
    """
    计算主题词分布
    :param topic_words_count: np.array [topic, word]
    :param beta: 
    :return: phi np.array [topic, word]
    """
    dim = topic_words_count.shape
    phi = np.zeros(dim)
    for i in range(dim[0]):
        sum_i = sum(topic_words_count[i, :]) + dim[1] * beta
        for j in range(dim[1]):
            phi[i, j] = (topic_words_count[i, j] + beta) * 1.0 / sum_i
    return phi


def gibbs_sampling(doc_topic_count, topic_words_count, doc_word, doc_word_topic, k, alpha, beta):
    doc_len = len(doc_word_topic)
    for i in range(doc_len):
        for j in range(len(doc_word[i])):
            doc_topic_count[i, doc_word_topic[i][j]] -= 1
            topic_words_count[doc_word_topic[i][j], doc_word[i][j]] -= 1
            p = np.zeros(k)
            for t_n in range(k):
                p[t_n] = ((doc_topic_count[i, t_n] + alpha)*1.0/(sum(doc_topic_count[i])+k*alpha))\
                         *((topic_words_count[t_n, doc_word[i][j]] + beta)*1.0/(sum(topic_words_count[t_n, :]) + k*beta))
                if t_n > 0:
                    p[t_n] += p[t_n-1]
            gs = random.random()*p[k-1]
            new_topic = 0
            while new_topic < k:
                if p[new_topic] > gs:
                    break
                new_topic += 1
            doc_topic_count[i, new_topic] += 1
            topic_words_count[new_topic, doc_word[i][j]] += 1
            # 不要忘记更新这个， 否则会出现负数
            doc_word_topic[i][j] = new_topic


def init_param_lda(doc_word, words_id, k):
    """
    LDA 初始化
    :param doc_word: 
    :param words_id: 
    :param k: 
    :return: 
    """
    doc_len = len(doc_word)
    word_len = len(words_id)
    doc_topic_count = np.zeros((doc_len, k))
    topic_words_count = np.zeros((k, word_len))
    doc_word_count = list()
    random.seed(0)
    # 随机初始化， 给每个文档每个词分配一个主题
    for i in range(doc_len):
        doc_word_count.append(list())
        for j in range(len(doc_word[i])):
            doc_word_count[i].append(random.randint(0, k-1))
    # 统计每个文档中， 主题出现的次数
    for i in range(doc_len):
        for j in range(len(doc_word_count[i])):
            doc_topic_count[i, doc_word_count[i][j]] += 1
    # 统计每个主题中， 词出现的次数
    for i in range(doc_len):
        for j in range(len(doc_word[i])):
            topic_words_count[doc_word_count[i][j], doc_word[i][j]] += 1
    return doc_topic_count, topic_words_count, doc_word_count


def load_word_id(wordsid_file):
    """
    加载词典，word对应的id
    :param wordsid_file: json格式的文件 
    :return:  {word:id}
    """
    with open(wordsid_file) as fopen:
        word_id = json.load(fopen)
    return word_id


def save_model(doc_topic, topic_words):
    """
    模型的保存
    :param doc_topic: 
    :param topic_words: 
    :return: 
    """
    np.save("./lda_model_data/doc2topic_lda.npy", doc_topic)
    np.save("./lda_model_data/topic2words_lda.npy", topic_words)


def load_model(doc_topic_file="./lda_model_data/doc2topic_lda.npy", topic_words_file="./lda_model_data/topic2words_lda.npy"):
    """
    模型的加载
    :param doc_topic_file: 
    :param topic_words_file: 
    :return: 
    """
    doc_topic = np.load(doc_topic_file)
    topic_words = np.load(topic_words_file)
    return doc_topic, topic_words


def predict(topic_words, words, words_id):
    w_id = list()
    for w in words:
        if w in words_id.keys():
            w_id.append(words_id[w])
        # w_id.append(words_id[w] if w in words_id.keys() else 0)
    dim = topic_words.shape[0]
    docn_topic = np.zeros(dim)
    for i in w_id:
        docn_topic = docn_topic + topic_words[:, i]
    docn_topic = docn_topic / sum(docn_topic)
    return docn_topic


if __name__ == "__main__":
    flag = 0
    if flag == 0:
        doc_list = [[1,2,2,3,3], [2,2,3,3,3,4], [5,6,6,6,7,7,7,7], [66,6,6,6,7,6,7,7,7,8,9]]
        wordsid_file = "./lda_model_data/word_id"
        doc_word, words_id = count_WordDoc(doc_list, wordsid_file)
        k = 2
        alpha = 0.1
        beta = 0.1
        iter_num = 20
        doc_topic, topic_words = lda_gibbs(doc_word, words_id, k, alpha, beta, 20)

    else:
        # test
        wordsid_file = "./lda_model_data/word_id"
        doc_topic, topic_words = load_model()
        print doc_topic
        print topic_words
        words = ['6', '6', '7', '7', '1', '4']
        words_id = load_word_id(wordsid_file)
        print predict(topic_words, words, words_id)

