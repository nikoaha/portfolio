"""
    Logistics System project utilizing SARSA in a trial environment
    Main Author: Niko Haapalainen, Tomoshi Iiyama - Tokyo Kosen project exchange
    Version 10 - 1st of March 2020
    
    This is my innovation project program developed in National Institute of Technology, Tokyo College
    between September 2019 to March 2020. 

    It is a logistics system, in which we simulated and experimented the performance of SARSA algorithm 
    from reinforcement learning area of machine learning. This is a single-agent version and it is 
    performing the way we wanted to - to deliver the foods between the coordinates (food facilities) and 
    keep the coordinates food stocks balanced.
    
    Note that this program is just a trial version study to experiment SARSA in this environment, which was
    my task back then.
    
    Developed with Python 3 in Jupyter Notebook environment.
    
"""

print(" \n \n #\n # PROGRAM STARTED!\n #\n\n")

#
# IMPORT LIBRARIES
#
import numpy as np
import random, math, time, sys
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
from IPython.display import clear_output
from copy import deepcopy as dc

#
# INITIATE ENVIRONMENT COORDINATES, ROUTES AND HYPERPARAMETERS
#
city = { 1: [1,1], 2: [2.5,2], 3: [4,10], 4: [2,9] }
cityRoutes = { 1: [2,3,4], 2: [1,3,4], 3: [1,2,4], 4: [1,2,3] }
alpha, gamma, epsilon = 0.1, 0.9, 0.1     # The learning rate, Worth of future rewards, For epsilon-greedy

#
# INITIATE ORDER CONSTRAINT
#
order_constraints = { 1: [2,3] } #2: [3,4]

#
# INITIATE MAXIMUM CARRYING CAPACITY (VOLUME) CONSTRAINT
#
give_foods = { 2: { 'banana_box': 1, 'apple_box': 0, 'melon_box': 0 },
               3: { 'banana_box': 0, 'apple_box': 1, 'melon_box': 1 },
               4: { 'banana_box': 1, 'apple_box': 0, 'melon_box': 1 }  }

get_foods = { 2: { 'banana_box': 0, 'apple_box': 0, 'melon_box': 0 },
              3: { 'banana_box': 1, 'apple_box': 0, 'melon_box': 0 },
              4: { 'banana_box': 0, 'apple_box': 3, 'melon_box': 0 }  }

stocks = { 2: { 'banana_box': 0, 'apple_box': 0, 'melon_box': 0 }, 
           3: { 'banana_box': 1, 'apple_box': 0, 'melon_box': 0 }, 
           4: { 'banana_box': 0, 'apple_box': 3, 'melon_box': 0 }  }

food_values = { 'banana_box': 5, 'apple_box': 10, 'melon_box': 20 }

food_on_truck = { 'banana_box': 2, 'apple_box': 2, 'melon_box': 2 }

cargo_volume, max_capacity = 0, 100
orig_give_foods, orig_get_foods, orig_stocks, orig_food_on_truck = dc(give_foods), dc(get_foods), dc(stocks), dc(food_on_truck)

#
# THIS DICTIONARY KEEPS A LIST WHETHER EACH OF THE CITIES ARE SATISFIED 
#
foods_delivered = { 2:0, 3:0, 4:0 }

#
# INITIALIZE Q FUNCTION
#
Q = {}
for state in range(1,5):             # 4 possible states, or cities
    for action in range(1,5):        # 4 possible actions
        Q[state, action] = 1 if (state != action) else 0

#
# CHOOSE RANDOM ACTION
#
def choose_rand_action(state):
    
    global cityRoutes, previous_choice
    routeList = cityRoutes[state]
    
    while True:
        num = random.randrange(0,3) # Random int between 0-3 ## 
        choice = routeList[num]
        
        print("choice_rand_action() generated: ", choice)
        
        if choice != 1 and foods_delivered[choice] == 0:            
            print("OK, proceeding with choice: ", choice)
            break
        
        elif check_deliveries():
            choice = 1
            print("Brute force, random choice: ", choice)
            break
            
        print("If this is printed, then no if-condition were satisfied in choose_rand_action().")     
    
    #print("previous_actions after choose_rand_action(): ", previous_actions)        
    
    return choice

#
# CHOOSE ACTION FROM Q FUNCTION
#
def maxAction(Q, state):
    global previous_actions
    values = np.array([Q[state,a] for a in range(1,5)])
    
    while True:
        action = np.argmax(values)
        action += 1 # corrects the indexation
        
        print("maxAction() generated: ", action)
        
        if state == action: # Prevent the agent from polling in its current state.
            values[action-1] = -1000
            continue
        
        if check_deliveries():
            action = 1
            print("All deliveries are satisfied, brute forcing to city 1...")
            break
            
        if action != 1 and foods_delivered[action] == 1:
            print("Chosen action destination [{}] ready satisfied! Continuing... ".format(action))
            values[action-1] = -1000
            continue
        
        elif action != 1 and foods_delivered[action] == 0:
            #past "not returning a city, where food exchange is completed" constraint
            previous_actions.append(action)
            break
            
        elif action == 1:
            print("Bad combination! Correcting...")
            values[action-1] = -1000
            continue
            
        print("If this is printed, then no if-condition were satisfied in maxAction().")       
            
    #print("previous_actions after maxAction(): ", previous_actions)
    
    return action

#
# UPDATE Q FUNCTION
#
def learn(state, action, reward, state_, action_):
    predict = Q[state, action]
    target = reward + gamma * Q[state_, action_]
    Q[state, action] = round(Q[state, action] + alpha * (target - predict), 1)

#
# APPEND THE CURRENT DISTANCE TO DISTANCE LIST
#
def addToDistanceList(state, action):
    
    def calcDistance(x1, y1, x2, y2):
        distance = math.sqrt((x2 - x1)**2 + (y2 - y1)**2)
        distance = round(distance, 2)
        return distance
    
    distanceList.append(calcDistance(city[state][0], city[state][1], city[action][0], city[action][1]))

#    
# RETURN TRUE IF ALL FOODS ARE DELIVERED IN A CITY (= IS THE CITY SATISFIED?)
#
def check_deliveries():
    return all(value == 1 for value in foods_delivered.values())

#
# CHECK CARGO SIZE - STOP PROGRAM IF MAX VOLUME EXCEEDED
#
def check_cargo_size():
    global cargo_volume
    for particle in list(food_on_truck):
        quantity, value = food_on_truck[particle], food_values[particle]
        for i in range(quantity):
            cargo_volume += value if cargo_volume+value <= max_capacity else sys.exit()
            #print(cargo_volume)
            
#
# FOR VIEWING THE LOG OF HISTORY OF CHOSEN PATHS
#
def list_history(listlog):
    print(" #\n #\n # START VISITEDCITIES DISPLAY\n")
    ye = 1
    for list_ in listlog:
        print("Episode {} ".format(ye), list_)
        ye += 1
        
def check_success(visitedCities):
    global success_rate, success_cof  
        
    if visitedCities != [1,2,3,4,1]:
        if visitedCities != [1,4,3,2,1]:
            success_cof -= 1
        
    pct = success_cof * 100 / total_episodes
    success_rate.append(pct)
    
#
# GIVE FOOD PARTICLE FROM AGENT TO THE CURRENT CITY
#
def food_exchange(city):
    global cargo_volume
    
    temp = give_foods[city]
    for particle in temp:
        if give_foods[city][particle] != 0:
            if food_on_truck[particle] != 0:
                
                food_on_truck[particle] -= temp[particle] #if food_on_truck[particle] != 0 else sys.exit()
                cargo_volume -= food_values[particle]
                stocks[city][particle] += temp[particle]
                give_foods[city][particle] -= temp[particle]
                #print("CARGO_VOL_GIVE: ", cargo_volume)
                
    temp = get_foods[city]
    for particle in temp:
        if get_foods[city][particle] != 0:
            
            quantity, value = get_foods[city][particle], food_values[particle] #1, 5
            for i in range(quantity):
                if cargo_volume+value <= max_capacity:
                    stocks[city][particle] -= 1
                    food_on_truck[particle] += 1
                    get_foods[city][particle] -= 1
                    cargo_volume += value
                    #print("CARGO_VOL_GET: ", cargo_volume)
    
    print("GetF: ", all(value == 0 for value in get_foods[city].values()))
    print("GiveF: ", all(value == 0 for value in give_foods[city].values()))
    
    if all(value == 0 for value in get_foods[city].values()) == True \
        and all(value == 0 for value in give_foods[city].values()) == True:
        foods_delivered[city] = 1
        print("Foods exchanged in city {} and is now satisfied!".format(city))
    else:
        print("Foods exchanged, but city {} is not satisfied yet.".format(city))
        
#
# PLOTS A REAL-TIME GRAPHICAL FIGURE
#
def plotter(curX, curY): #huutis
    
    global city
    x = [city[1][0], city[2][0], city[3][0], city[4][0]]
    y = [city[1][1], city[2][1], city[3][1], city[4][1]]
    
    clear_output()
    plt.plot(x, y, 'ko')
    plt.plot(curX, curY, 'ro')
    plt.text(1.1, 1, "facility 1, START")
    plt.text(2.6, 2, "facility 2: {}".format(stocks[2]))
    plt.text(4, 9.35, "facility 3: {}".format(stocks[3]))
    plt.text(2.05, 8.4, "facility 4: {}".format(stocks[4]))
    #plt.text(1.1, 1, "city 1, START")
    #plt.text(2.6, 2, "city 2: {}".format(stocks[2]))
    #plt.text(4, 9.35, "city 3: {}".format(stocks[3]))
    #plt.text(2.05, 8.4, "city 4: {}".format(stocks[4]))
    plt.text(0.85, -2, "food on truck: {}".format(food_on_truck))
    #plt.text(0.85, -2.7, "food on truck: {}".format(food_on_truck))
    plt.show()
    print("GIVE FOODS: \n", give_foods[2], "\n", give_foods[3], "\n", give_foods[4])
    print("\n")
    print("GET FOODS: \n", get_foods[2], "\n", get_foods[3], "\n", get_foods[4])
    print("\n")
    print("STOCKS: \n", stocks[2], "\n", stocks[3], "\n", stocks[4])
    print("\n")
    time.sleep(3.0)
    clear_output()

#
# PRINT FINAL REPORT
#
def final_report():

    clear_output()
    
    ### Q-VALUE PRINT
    
    print("\n\n\nFINAL REPORT: state (vertical), action (horizontal)")
    for state in range(1,5):
        print()
        for action in range(1,5):
            print(Q[state, action], end = ' ')

    ### PRINT GRAPH 1 - PERFORMANCE IN PERCENTS

    #plt.plot(success_rate, 'b--')
    #plt.xlabel('Episode')
    #plt.ylabel('Success rate (%)')
    #plt.title('Performance overview')
    #x1,x2,y1,y2 = plt.axis()
    #plt.axis((x1,x2,-5,105))
    #plt.show()
    
    ### PRINT GRAPH 1 ALTERNATE - DISTANCE PROGRESSION
    
    plt.clf()
    plt.plot(distListPlot, 'b--')
    plt.title('PERFORMANCE REPORT')
    plt.xlabel('Episodes')
    plt.ylabel('Distance')
    plt.show()
    
    plt.clf()

    # PRINT GRAPH 2
    
    value_candidates = np.unique(distListPlot) # 20.24 20.52 28.32 32.02 33.01
    x_pos = np.arange(len(value_candidates)) 
    results = []

    for value in value_candidates:
        results.append(distListPlot.count(value) * 100 / total_episodes)
        
    plt.bar(x_pos, results, align='center')
    plt.xticks(x_pos, value_candidates)
    plt.xlabel('Distance')
    plt.ylabel('Frequency (%)')
    plt.title('Overview on travelled distances')
    x1,x2,y1,y2 = plt.axis()
    plt.axis((x1,x2,0,105))
    
    count, y_axis = 0, 80
    plt.text(5, y_axis, "% on each result:")
    y_axis -= 10
    for unique in value_candidates:
        plt.text(5, y_axis, "{}: {}%".format(unique, results[count]))
        count += 1
        y_axis -= 10
    
    plt.show()

#
# INITIALIZE INSTANCE VARIABLES, FLAGS, CONSTANTS AND LISTS
#
total_episodes, max_steps, rewards, prev_distance, listlog = 200, 10, 0, 10000, []
total_rewards, distanceList, visitedCities, distListPlot, previous_actions = [], [], [], [], []
success_rate, success_cof, routes_index = [], total_episodes, [0,1,2]
check_cargo_size() # そもそもオバーフローないの為に出荷のボリュームをチェックする。

#
# TOOLS FOR THE ALGORITHM LOOP
#
def give_rewards(ep):
    global prev_distance, rewards, total_rewards
    new_distance = sum(distanceList)

    if new_distance <= prev_distance:
            rewards += 10
            print("Is shorter route >>> received +10 rewards!")
            prev_distance = new_distance
            print("prev_distance is now: ", prev_distance)
    
    elif new_distance > prev_distance:
        if rewards-5 >= 0: 
            rewards -= 5
            print("Is longer route >>> received -5 penalty!")

    else: print("If this is printed, then no rewards option was chosen!")

    print("\n\n\n")
    
#
# RESET INITALIZATION INSTANCE VALUES FOR NEXT EPISODE LOOP
#
def reset():
    global distanceList, visitedCities, previous_actions, cargo_volume, routes_index
    global rewards, give_foods, get_foods, stocks, food_on_truck, total_episodes
    distListPlot.append(round(sum(distanceList), 2))
    order_constraints.update({1: [2,3]})
    foods_delivered.update({2:0, 3:0, 4:0})
    total_rewards.append(rewards)
    distanceList, visitedCities, previous_actions, cargo_volume = [], [], [], 0
    give_foods, get_foods, stocks, food_on_truck = dc(orig_give_foods), dc(orig_get_foods), dc(orig_stocks), dc(orig_food_on_truck)
    check_cargo_size()
    
#
# START ALGORITHM LOOP
#
for episode in range(total_episodes):
    
    print("  --- Episode {} commencing! ---  \n".format(episode+1))
    print("  --- Step {} commencing! ---  \n".format(1))
        
    state = 1
    action = maxAction(Q, state) if np.random.random() < epsilon else choose_rand_action(state)
    
    visitedCities.append(state)
    addToDistanceList(state, action)
    
    ##### PLOTTER #####
    if (episode > total_episodes-3): plotter(city[state][0], city[state][1]) # Plotter
    #plotter(city[state][0], city[state][1]) # Plotte
    
    for step in range(max_steps):
        
        if step != 0: print("\n --- Step {} commencing! ---  \n".format(step+1))
        
        state_ = action
        visitedCities.append(state_)
        
        print("The agent is in city: ", state_)
        print("FD bef: ", foods_delivered)
        if state_ != 1: food_exchange(state_)
        print("FD now: ", foods_delivered)
        
        ##### PLOTTER #####
        if (episode > total_episodes-3): plotter(city[state_][0], city[state_][1]) # Plotter
        #plotter(city[state_][0], city[state_][1]) # Plotter
        
        action_ = maxAction(Q, state_) if np.random.random() < epsilon else choose_rand_action(state_) 

        print("\n[",state,",", action,"]")
        print("[",state_,",", action_,"]")
        print("Visited cities list: {}\n".format(visitedCities))

        if (state_ == 1 and check_deliveries() == True):
            give_rewards(episode)
            break

        learn(state, action, rewards, state_, action_)
        addToDistanceList(state_, action_)
        state, action = state_, action_
        print("\n\n\n")
    
    check_success(visitedCities)
    listlog.append(visitedCities)
    reset()
    epsilon = epsilon + 3 / total_episodes if epsilon < 1.0 else 1.0
    print("Episode done! Epsilon: {} \n\n\n\n\n\n\n\n".format(epsilon))

list_history(listlog)
final_report()

#
#
# END OF CODE
