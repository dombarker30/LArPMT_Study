import smtplib
import os
import sys
from email.MIMEMultipart import MIMEMultipart
from email.MIMEText import MIMEText
    
directory = os.getcwd()
file = open(directory+"/tmpfile.txt","r")
file_text = file.read()

server = smtplib.SMTP('smtp.gmail.com', 587)
server.starttls()
server.login("dominic.barker@sheffield.ac.uk", "sdxtnt5f")

msg = MIMEMultipart()
msg['From'] = "dominic.barker@sheffield.ac.uk"
msg['To'] = "dominic.barker@sheffield.ac.uk"

if int(sys.argv[1]) == 0:
    msg['Subject'] = "DAQ Completed successfully"
else: 
    msg['Subject'] = "DAQ Crashed"

body = "The DAQ is done with info: \n" + file_text     
msg.attach(MIMEText(body, 'plain')) 

text = msg.as_string()
server.sendmail("dominic.barker@sheffield.ac.uk", "dominic.barker@sheffield.ac.uk",text)
server.quit()
