import urllib.request
import time
from html.parser import HTMLParser

import SendGmail
import localIni

strFinalMessage = ''
CompanyInfo = {}
bToday = False
bFirstSignal = False

class MyHTMLParser(HTMLParser):

    bLastSignalFound = False
    bLastCloseFound = False
    bChangeFound = False
    bChangePercentFound = False
    bLastSignalDate = False
    bFirstTableElementFound = False

    def handle_starttag(self, tag, attrs):
        for a, b in attrs:
            if a == 'id':
                if b == 'MainContent_LastSignal':
                    self.bLastSignalFound = True
                if b == 'MainContent_LastClose':
                    self.bLastCloseFound = True
                if b == 'MainContent_Change':
                    self.bChangeFound = True
                if b == 'MainContent_ChangePercent':
                    self.bChangePercentFound = True
                if b == 'MainContent_signalpagehistory_PatternHistory24_DXDataRow0':
                    self.bLastSignalDate = True

    def handle_data(self, data):
        global strFinalMessage
        if self.bLastSignalFound:
            CompanyInfo['LastSignal'] = data
            self.bLastSignalFound = False
        if self.bLastCloseFound:
            CompanyInfo['LastClose'] = data
            self.bLastCloseFound = False
        if self.bChangeFound:
            CompanyInfo['Change'] = data
            self.bChangeFound = False
        if self.bChangePercentFound:
            CompanyInfo['PercentChange'] = data
            self.bChangePercentFound = False
        if self.bFirstTableElementFound:
            CompanyInfo['DateOfSignal'] = data
            self.bFirstTableElementFound = False
            self.bLastSignalDate = False
        if self.bLastSignalDate:
            self.bFirstTableElementFound = True

def DisplayStatus(Company):
    url = "https://www.americanbulls.com/SignalPage.aspx?lang=en&Ticker=" + Company
    myPage = urllib.request.urlopen(url)
    parser = MyHTMLParser()
    parser.feed(myPage.read().decode("utf-8"))

def GetCompanyInfo(Signal):
    Info = CompanyInfo[Signal]
    return Info

def CreateMessage(Company, bGetPrevious = True):
    global bToday
    global bFirstSignal
    SIGNAL_TIME = CompanyInfo['DateOfSignal']
    bToday = time.strftime("%m/%d/%Y") == SIGNAL_TIME
    bToday = True
    print(SIGNAL_TIME)
    global strFinalMessage
    strFinalMessage += GetCompanyInfo('DateOfSignal')   + "\n"
    strFinalMessage += GetCompanyInfo('LastSignal')     + "\n"
    strFinalMessage += GetCompanyInfo('LastClose')      + "\n"
    strFinalMessage += GetCompanyInfo('Change')         + "\n"
    strFinalMessage += GetCompanyInfo('PercentChange')
    if bToday and bGetPrevious:
        strFinalMessage += "\n" + localIni.GetPreviousStockSignal(Company)
        LAST_KNOWN_SIGNAL = localIni.GetPreviousStockSignal(Company)
        CURRENT_SIGNAL = CompanyInfo['LastSignal']
        if CURRENT_SIGNAL not in LAST_KNOWN_SIGNAL:
            bFirstSignal = True
            localIni.StoreStockInfo(Company, CompanyInfo['LastSignal'])

def DailyRoutine():
    global strFinalMessage
    global bFirstSignal
    MAIN_USER = localIni.GetEmailUserName()
    MAIN_PASSWORD = localIni.GetEmailPassword()
    print( localIni.GetClients())
    for User in localIni.GetClients():
        Companies = localIni.GetCompanies(User).split(',')
        for Company in Companies:
            strFinalMessage += Company + ":\n"
            DisplayStatus(Company)
            CreateMessage(Company)
            print(Company)
            if bToday and bFirstSignal:
                SendGmail.send_GMail(MAIN_USER, MAIN_PASSWORD, localIni.GetSMSEmail(User),'',strFinalMessage)
                SendGmail.send_GMail(MAIN_USER, MAIN_PASSWORD, User,'Signal Change',strFinalMessage)
                print(strFinalMessage)
                bFirstSignal = False
            strFinalMessage = ''

def AllRoutine(REQUESTED_USER):
    global strFinalMessage
    MAIN_USER = localIni.GetEmailUserName()
    MAIN_PASSWORD = localIni.GetEmailPassword()
    print( localIni.GetClients())
    for User in localIni.GetClients():
        if User != REQUESTED_USER:
            continue
        Companies = localIni.GetCompanies(User).split(',')
        for Company in Companies:
            strFinalMessage += Company + ":\n"
            DisplayStatus(Company)
            CreateMessage(Company, False)
            SendGmail.send_GMail(MAIN_USER, MAIN_PASSWORD, localIni.GetSMSEmail(User),'',strFinalMessage)
            strFinalMessage = ''
