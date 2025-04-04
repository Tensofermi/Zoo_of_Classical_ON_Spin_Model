import header_footer as hf
import os
import time
import subprocess
from concurrent.futures import ThreadPoolExecutor

start_time = time.time()
##################################################################################
# Define the constants
MODE = hf.mode
TARGET_FILE = hf.format_file
SCRIPT = 'format_data.py'
RUN = 'python3'

# Define the path to search
search_path = os.path.join('..', '..', MODE, 'jobs')

# Function to run the script on a given file
def run_script_on_file(file_path):
    subprocess.run([RUN, SCRIPT, file_path])

# Create a ThreadPoolExecutor to run tasks in parallel
with ThreadPoolExecutor() as executor:
    # Walk through the directory tree
    for root, dirs, files in os.walk(search_path):
        for file in files:
            if file == TARGET_FILE:
                file_path = os.path.join(root, file)
                # Check if the file is indeed a file and not a symbolic link, etc.
                if os.path.isfile(file_path):
                    # Submit tasks to the executor for parallel processing
                    executor.submit(run_script_on_file, file_path)

##################################################################################
end_time = time.time()
execution_time = end_time - start_time
print(f"Time for formatting  data: {execution_time:.4f} seconds")