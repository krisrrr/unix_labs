from subprocess import PIPE, Popen
import time, os, csv


TIME_FILENAME = 'times.csv'
SCRIPT_PATH = os.path.dirname(os.path.realpath(__file__))
START_TIME = time.time()

dbus_proc = Popen(["dbus-monitor", "--system",
                   "sender=org.freedesktop.NetworkManager, path=/org/freedesktop/NetworkManager, member=StateChanged"],
                  stdout=PIPE,
                  stderr=PIPE)

def main():
    while True:
        k = dbus_proc.stdout.readline()
        words = k.decode('UTF-8').split()
        try:
            element_index = words.index('uint32')
        except ValueError:
            continue

        conn_code = int(words[element_index + 1])
        print('Got change state with code:', conn_code)

        if conn_code == 70:
            START_TIME = time.time()
        if (conn_code == 20 or conn_code == 40) and START_TIME is not None:
            output_file = open(os.path.join(SCRIPT_PATH, TIME_FILENAME), 'a')
            csv_writer = csv.writer(output_file)
            csv_writer.writerow([START_TIME, time.time()])
            START_TIME = None
            output_file.flush()
            output_file.close()


if __name__ == "__main__":
    main()
