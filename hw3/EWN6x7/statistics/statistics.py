import pandas as pd

steps = {}

with open('./statistics/log.txt', 'r') as f:
    s = f.read()
    lines = s.split('\n')

    for line in lines:
        step = int(line[0])
        time = int(line[2:])
        if step not in steps:
            steps[step] = []
        steps[step].append(time)

steps = {k: v for k, v in sorted(steps.items(), key=lambda item: item[0])}
statistics = []
step_time_x = []
step_time_y = []
for step, times in steps.items():
    count = len(times)
    average = sum(times) / len(times)
    mn_time = min(times)
    mx_time = max(times)
    step_time_x.append(step)
    step_time_y.append(average)
    statistics.append([step, count, average, mn_time, mx_time])

df = pd.DataFrame(statistics, columns=['step', 'count', 'average', 'min', 'max'])
pd.options.display.float_format = '{:.2f}'.format
print(df)
df.to_csv('./statistics/step_statistics.csv', index=False)

import matplotlib.pyplot as plt

fig, ax = plt.subplots()
ax.plot(step_time_x, step_time_y)
ax.ticklabel_format(useOffset=False, style='plain')
plt.xlabel('Number of steps')
plt.ylabel('Average time (ms)')
plt.title('Average time for each number of steps(ms)')
plt.show()