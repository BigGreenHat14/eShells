# eShells Toy Currency
eShells is an open source, **toy** currency that was made to demonstrate [WebDB](https://github.com/SeafoodStudios/webdb)'s capabillities.

eShells has a web based wallet, although you must use HTTPS and to never use real usernames or password. This is because we want to prevent any data interceptions. Vercel logs the usernames and passwords, which further enforces why you should never use your real data on this platform.
The wallet is here: [https://eshells.seafoodstudios.com/](https://eshells.seafoodstudios.com/)

eShells has a miner too, although it does not earn you real money, it only earns you eShells. It also uses around half of your CPU when mining, so never run it 24/7. You should also inspect the [source code](https://raw.githubusercontent.com/SeafoodStudios/eShells/refs/heads/main/src/miner.py) if you are suspicious or feeling cautious.
To download the miner, run this command in your terminal:
```
curl https://raw.githubusercontent.com/SeafoodStudios/eShells/refs/heads/main/src/miner.py -o miner.py
```
To run the miner, run this command in your terminal (after downloading):
```
python3 miner.py
```

Please use eShells carefully, and to never use it for production purposes.
