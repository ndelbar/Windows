try:
    from configparser import ConfigParser
except ImportError:
    from ConfigParser import ConfigParser  # ver. < 3.0

def AddList(strEmail,strList):

    # instantiate
    config = ConfigParser()

    # parse existing file
    config.read('UserInfo.ini')

    # read values from a section
    #string_val = config.get('section_a', 'string_val')
    #bool_val = config.getboolean('section_a', 'bool_val')
    #int_val = config.getint('section_a', 'int_val')
    #float_val = config.getfloat('section_a', 'pi_val')

    # update existing value
    #config.set('section_a', 'string_val', 'world')

    # add a new section and some values

    if not config.has_section(strEmail):
        config.add_section(strEmail)
        
    config.set(strEmail, 'UserName', 'Nicholas')
    config.set(strEmail, 'StockList', strList)

    # save to a file
    with open('UserInfo.ini', 'w') as configfile:
        config.write(configfile)

def GetEmailUserName():
    config = ConfigParser()
    config.read('LoginInfo.ini')
    return config.get('stock.sms.helper','username')

def GetEmailPassword():
    config = ConfigParser()
    config.read('LoginInfo.ini')
    return config.get('stock.sms.helper','password')

def GetMainSMSEmail():
    config = ConfigParser()
    config.read('LoginInfo.ini')
    return config.get('stock.sms.helper','MainSMSEmail')

