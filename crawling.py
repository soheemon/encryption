#!/usr/bin/python
#-*- coding: utf-8 -*-

from selenium import webdriver
from bs4 import BeautifulSoup
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC

import os
import shutil
from selenium.webdriver.chrome.options import Options

import re

import sys
reload(sys)
sys.setdefaultencoding('utf-8')

def mk_folder(rootPath, subtName) :

	new_folder_path = rootPath + subtName + '/' 

	if not os.path.exists(new_folder_path):
		os.mkdir(new_folder_path)
	return new_folder_path

def mv_file(DownloadfolderPath, dest) :
	fileList = os.listdir(DownloadfolderPath)

	for fileName in fileList:
		shutil.move(DownloadfolderPath + fileName, dest + fileName)

rootPath = '/home/soheemon/Downloads/'
DownloadfolderPath = mk_folder(rootPath, 'work')

#다운로드 Path 설정
chrome_options = webdriver.ChromeOptions()
preferences = {"download.default_directory": DownloadfolderPath,
				"directory_upgrage": True,
				"safebrowsing.enabled": True}
chrome_options.add_experimental_option("prefs", preferences)

driver = webdriver.Chrome(chrome_options=chrome_options, executable_path=r'/home/soheemon/Downloads/chromedriver')
driver.get('https://ucampus.knou.ac.kr/ekp/user/login/retrieveULOLogin.do?logoutYn=logout&strReturnUrl=https%3A%2F%2Fucampus.knou.ac.kr%2Fekp%2Fuser%2Flogin%2FretrieveULOLogin.do')

#로그인
username = ''
password = ''
driver.find_element_by_name('username').send_keys(username)
driver.find_element_by_name('password').send_keys(password)

driver.find_element_by_xpath('//*[@id="loginTab"]/fieldset/div[4]/button').click()

#과목이름 가져오기
html = driver.page_source
soup = BeautifulSoup(html, 'html.parser')
lectures = soup.select('div.lecture-progress-item-header > div.lecture-info > h3 > a')
#To.do 과목도 반복문으로 처리하기.

#과목 클릭
subjList = driver.find_elements_by_css_selector('div.lecture-progress-item-header > div.lecture-info > h3 > a')
subjList[0].click()

#강의자료 버튼 클릭
lectureData = driver.find_elements_by_css_selector('div.lecture-progress-item-body > div.btns.text-right > button:nth-child(2)')
lectureData[0].click()

#select 설정
#기출문제: 3 기출문제정답: 4 
select = 3

#출석수업 대체시험: 1 기말고사: 2
solutionKindCode = 1

if solutionKindCode == 1:
	solutionKindName = '대체시험'
elif solutionKindCode == 2:
	solutionKindName = '기말고사'

p = re.compile(unicode(solutionKindName))

driver.find_element_by_css_selector('#searchCodeType > option:nth-child(3)').click()
#페이지 설정
page_length = len(driver.find_elements_by_css_selector('#pager > a'))
for i in range(1, page_length + 1):
	page_selector = '#pager > a:nth-child(' + str(i) + ')'
	WebDriverWait(driver, 20) \
		.until(EC.presence_of_element_located((By.CSS_SELECTOR, page_selector))).click()	

	length = len(driver.find_elements_by_css_selector('#itemsList > tr > td.text-left > a'))
#글 클릭
	for i in range(1, length + 1):
		selector = '#itemsList > tr:nth-child(' + str(i) + ') > td.text-left > a'
		title = WebDriverWait(driver, 30) \
			.until(EC.presence_of_element_located((By.CSS_SELECTOR, selector)))

		#solutionKindName에 따라서 다운로드
		m = p.search(unicode(title.text))
		if m:
			title.click()

			#다운로드 버튼 클릭
			driver.find_element_by_xpath('//*[@id="uploaded-files1"]/ul/li[1]/a').click()
			print('sleeeeeeeeeeeeeeeep')

			driver.find_element_by_xpath('//*[@id="lectureTab6"]/div/div[2]/div/div[2]/a').click()

#다운로드가 끝나면 rootPath밑에 과목이름으로 폴더를 만든다.
subjName = unicode(lectures[0].text)
subjFolderPath = mk_folder(rootPath, subjName)

#파일 전부 이동
mv_file(DownloadfolderPath, subjFolderPath)
