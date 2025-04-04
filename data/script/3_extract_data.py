import header_footer as hf
import time
import argparse
import itertools

start_time = time.time()
##################################################################################
# Function to combine multiple files
def combine_files(file_list):
    data = []
    headers_added = False
    headers = []

    for file in file_list:
        with open(file, 'r') as f:
            lines = f.readlines()
            if not headers_added:
                headers = [line for line in lines if line.strip().startswith("#")]
                headers_added = True
            data += [line for line in lines if not line.strip().startswith("#") and line.strip() != '']

    return headers, data

# Function to sort by an arbitrary number of fields
def sort_by_fields(lines, fields):
    return sorted(lines, key=lambda line: tuple(float(line.split()[field]) for field in fields))

# Function to segregate rows based on selected entry indices
def segregate_rows_by_entry(input_data, entry_indices):
    rows_dict = {}

    for line in input_data:
        stripped_line = line.strip()

        if stripped_line:
            entries = stripped_line.split('\t')
            combined_key = tuple(entries[index] for index in entry_indices)

            if combined_key not in rows_dict:
                rows_dict[combined_key] = []
            rows_dict[combined_key].append(line)

    return rows_dict

# Function to write rows to separate files
def write_rows_to_files(headers, rows_dict, variable_names, decimal_places):
    for key, rows in rows_dict.items():
        # Create a formatted name based on the changing variables
        variable_info = "_".join([f"{name}_{float(key[idx]):.{decimal_places[idx]}f}" for idx, name in enumerate(variable_names)])
        formatted_name = f"../raw/raw_{variable_info}.dat"

        with open(formatted_name, 'w') as outfile:
            outfile.writelines(headers)
            outfile.writelines(rows)


##############################################
# Define the indices as a list for flexibility, including decimal precision
indices = hf.indices
##############################################

# Parse input arguments
parser = argparse.ArgumentParser(description="Combine multiple data files.")
parser.add_argument('input_files', type=str, nargs='+', help="Input files to be combined")

args = parser.parse_args()

# Combine input files
headers, data = combine_files(args.input_files)

fields_sortby = [indices[index][1] for index in range(len(indices))]

sorted_data = sort_by_fields(data, fields_sortby)

# Loop through combinations of indices with r = N - 1 (where N is the number of indices)
r = len(indices) - 1
for combination in itertools.combinations(indices, r):
    # print(combination)

    # Extract indices for sorting and segregating
    changing_indices = [index for name, index, _ in combination]
    changing_names = [name for name, index, _ in combination]
    decimal_places = [places for name, index, places in combination]

    # Segregate rows based on changing entry indices
    rows_dict = segregate_rows_by_entry(sorted_data, changing_indices)

    # Write segregated rows to separate files
    write_rows_to_files(headers, rows_dict, changing_names, decimal_places)
        
##################################################################################
end_time = time.time()
execution_time = end_time - start_time
print(f"Time for extracting  data: {execution_time:.4f} seconds")