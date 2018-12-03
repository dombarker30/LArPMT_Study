import smtplib
import os
import sys
from email.MIMEMultipart import MIMEMultipart
from email.MIMEText import MIMEText
    

file = open("/home/argonshef/CAENHV/HVtmpfile.txt","r")
file_text = file.read()

server = smtplib.SMTP('smtp.gmail.com', 587)
server.starttls()
server.login("dominic.barker@sheffield.ac.uk", "sdxtnt5f")

msg = MIMEMultipart()
msg['From'] = "dominic.barker@sheffield.ac.uk"
msg['To'] = "dominic.barker@sheffield.ac.uk"
msg['Subject'] = "Problem With the High Voltage"

body = "Voltage is not correct with this mointor value: \n" + file_text     
msg.attach(MIMEText(body, 'plain')) 

text = msg.as_string()
server.sendmail("dominic.barker@sheffield.ac.uk", "dominic.barker@sheffield.ac.uk",text)
server.quit()
