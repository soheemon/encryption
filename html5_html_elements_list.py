#-*- coding: utf-8 -*-
from selenium import webdriver

driver = webdriver.Chrome(executable_path='/home/soheemon/Downloads/chromedriver')
driver.maximize_window()
driver.get("https://www.w3.org/TR/html5/")

#code 노드들 가져오기
code_nodes =  driver.find_elements_by_css_selector('#toc > ol:nth-child(2) > li:nth-child(4) >\
		ol:nth-child(2) > li > ol > li > a > span.content > span > code')

#Webelement요소들을 text순으로 정렬하기 위해
#text list를 만든다.
code_list = list()
for code_node in code_nodes:
	code_list.append(code_node.text)

#정렬
code_list.sort()

#출력
for html_element_name in code_list:
	print(html_element_name)
