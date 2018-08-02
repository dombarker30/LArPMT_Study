import smtplib
 
server = smtplib.SMTP('smtp.gmail.com', 587)
server.starttls()
server.login("dominic.barker@sheffield.ac.uk", "sdxtnt5f")
 
msg = "The DAQ is done"
server.sendmail("dominic.barker@sheffield.ac.uk", "dominic.barker@sheffield.ac.uk", msg)
server.quit()
