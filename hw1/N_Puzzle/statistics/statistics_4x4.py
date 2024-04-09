import pandas as pd

total_time = 0

datas = []
steps = {}

def extract_time(lines):
    for line in lines:
        if (line.find('Time') == -1):
            continue
        line = line.replace('Time:', '').replace('ms', '').replace(' ', '')
        return int(line)
    return None

def extract_step(line):
    for line in lines:
        if (line.find('Number of steps:') == -1):
            continue
        line = line.replace('Number of steps:', '').replace('ms', '').replace(' ', '')
        return int(line)
    return None

def extract_state(lines):
    for line in lines:
        if (line.find('Explored state:') == -1):
            continue
        line =  line.replace('Explored state:', '').replace(' ', '')
        return int(line)
    return None

for i in range(1000):
    with open(f'./output/1726539273_4x4/random_board_{i}.out', 'r') as f:
        s = f.read()
        lines = s.split('\n')

        time = extract_time(lines)
        if (time != None):
            total_time += time
        step = extract_step(lines)
        node = extract_state(lines)
        if (step != None and node != None and time != None):
            if (step not in steps):
                steps[step] = {}
                steps[step]['time'] = []
            steps[step]['time'].append(time)
            if ('node' not in steps[step] and node >= 0):
                steps[step]['node'] = []
            if (node >= 0):
                steps[step]['node'].append(node)
        if (time != None and step != None):
            datas.append([i, step, time, node])

df = pd.DataFrame(datas, columns=['number', 'step', 'time', 'node'])
df.to_csv('./statistics/statistics_data.csv', index=False)

print(f'Total time: {total_time}ms')
print(f'Average time: {total_time/len(datas)}ms')
print(f'Solved: {len(datas)}')
print(f'Unsolvable: {1000 - len(datas)}')

steps = {k: v for k, v in sorted(steps.items(), key=lambda item: item[0])}
step_time_x = []
step_node_x = []
time_y = []
node_y = []
statistics_time = []
statistics_node = []

for step in steps:
    statistics_time.append([step, len(steps[step]['time']), min(steps[step]['time']), max(steps[step]['time']), sum(steps[step]['time'])/len(steps[step]['time']), ])
    if ('node' in steps[step]):
        statistics_node.append([step, len(steps[step]['node']), min(steps[step]['node']), max(steps[step]['node']), sum(steps[step]['node'])/len(steps[step]['node']), ])
    step_time_x.append(step)
    time_y.append(sum(steps[step]['time'])/len(steps[step]['time']))
    if ('node' in steps[step]):
        step_node_x.append(step)
        node_y.append(sum(steps[step]['node'])/len(steps[step]['node']))

df = pd.DataFrame(statistics_time, columns=['step', 'count', 'min', 'max', 'avg'])
pd.options.display.float_format = '{:.2f}'.format
print(df)
df.to_csv('./statistics/time_statistics.csv', index=False)

import matplotlib.pyplot as plt

fig, ax = plt.subplots()
ax.plot(step_time_x, time_y)
ax.ticklabel_format(useOffset=False, style='plain')
plt.xlabel('Number of steps')
plt.ylabel('Average time (ms)')
plt.title('Average time for each number of steps(ms)')
plt.show()

df = pd.DataFrame(statistics_node, columns=['step', 'count', 'min', 'max', 'avg'])
pd.options.display.float_format = '{:.2f}'.format
print(df)
df.to_csv('./statistics/node_statistics.csv', index=False)

fig, ax = plt.subplots()
ax.plot(step_node_x, node_y)
ax.ticklabel_format(useOffset=False, style='plain')
plt.xlabel('Number of steps')
plt.ylabel('Average node')
plt.title('Average node for each number of steps')
plt.show()