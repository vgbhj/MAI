import json

n, m = map(int, input().split())
trigger = []
shipment = []


for i in range(n):
    d = input().split()
    a,b = map(int, d[:2])
    for i in d[2:2+a]:
        trigger.append(i)
    for i in d[2+a:2+a+b]:
        shipment.append(i)

# print(trigger)
# print(shipment)

data_j = []
data_id = []

for i in range(m):
    j = json.loads(input())
    j_offer = j["offer"] 
    if(not(j_offer["id"] in data_id)):
        data_id.append(j_offer["id"])
        data_j.append(j["offer"])
        print(data_j[0]["id"])
    else:
        for i in j_offer:
            print(i)