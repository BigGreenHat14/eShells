import requests
import hashlib
import random
import time

def hash(string: str) -> str:
    return hashlib.sha256(string.encode()).hexdigest()

print("eShell Miner")
print("by SeafoodStudios")
print("To login, signup and/or access your account, go here:")
print("https://eshells.seafoodstudios.com/\n")
print("eShells is a toy currency, and this miner will only gain you eShells, not real world money.")
print("This program will use a large amount of your computation power. By using this program, you consent to this.")
print("You may quit at any time by pressing Control + C, or closing the window.")
print("This file is located in your main directory if you downloaded it from our GitHub. Do not download a miner from anywhere you don't trust.")
print("Thank you for using eShells, and we hope you have a good experience.")
      
start = ""
while True:
    while not start == "mine" and not start == "exit":
        start = str(input("Would you like to begin mining or exit (mine/exit)? "))
    if start == "mine":
        address = str(input("Where would you like to send the results of the mining? "))
        print("Mining has begun. To exit, press Control + C, or close the window.")
        count = 0
        while True:
            if count % 1000000 == 0:
                if random.randint(1,10) == 1:
                    print(f"""{count} hashes have been attempted.""")
                time.sleep(1)
            try:
                theproof = str(random.randint(1,1000000000000))
                thehash = str(hash(theproof))
                if thehash.startswith("0000000"):
                    time.sleep(10)
                    response = requests.get(f"""https://eshells.pythonanywhere.com/submit_hash/{address}-{thehash}-{theproof}""")
                    print("\nNew Hash Found!")
                    print(response.text)
                    print(f"""Technical data: {thehash} was made from {theproof}.""")
            except Exception as e:
                print("Error: " + str(e))
            count += 1
    elif start == "exit":
        print("Exiting...")
        break
    else:
        print("Error: Input Error")
