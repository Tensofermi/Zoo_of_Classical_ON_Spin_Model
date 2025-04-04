import header_footer as hf
import math
import time
import sys

start_time = time.time()
##################################################################################
# Function to compress Monte Carlo data
# The compression process involves calculating the average of multiple entries
# that belong to the same group defined by unique values of specified indices (e.g., L, beta, K).
# Averaging is done using inverse variance weighting, where values with lower errors contribute more.
# Error propagation is handled by calculating the weighted standard error of the mean.

def compress(f_name, wid, eps, header, footer, indices, seed_index):
    with open(f_name, 'r') as rf:
        with open(f_name.replace('../raw/raw_', '../cmp/cmp_'), 'w') as wf:
            wf.write(rf.readline())  # Copy column index
            wf.write(rf.readline())  # Copy observation name
            line_now = [float(element) for element in rf.readline().split()]
            num_col = len(line_now)
            
            # Initialize variables for data processing
            num_term = 0
            lines_data, num_member, start_index = {}, {}, {}
            index = 0
            current_flags = [0] * len(indices)
            
            # Read and categorize lines based on unique values of indices
            while True:
                lines_data[index] = line_now  # Store line data
                # This condition checks if the current line belongs to the same term by comparing all specified indices.
                # This ensures that the same values across selected indices are grouped together for compression.
                if all(line_now[index_val] == current_flags[i] for i, (_, index_val, _) in enumerate(indices)):
                    num_member[num_term - 1] += 1
                else:  # Update flags and initialize new term
                    num_term += 1
                    num_member[num_term - 1] = 1
                    start_index[num_term] = index
                    current_flags = [line_now[index_val] for _, index_val, _ in indices]
                # Get next line
                try:
                    line_now = [float(element) for element in rf.readline().split()]
                    if not line_now:
                        break
                except ValueError:
                    break
                index += 1
            
            # Compress data
            for i in range(num_term):
                context = ""
                si = start_index[i + 1]

                # Write header information
                for j in range(header):
                    if j == seed_index:  # Fill seed with number of repetitions
                        context += '\t' + str(num_member[i]) + 'x'.ljust(wid)
                        continue
                    context += '\t' + str(lines_data[si][j]).ljust(wid)

                # Write data or mean values
                if num_member[i] == 1:
                    for j in range(header, num_col):
                        context += '\t' + str(format(lines_data[si][j], ".16f")).ljust(wid)
                else:
                    # Here, `quan` and `err` are initialized for accumulating weighted values and errors.
                    # This implements a weighted average where inverse variances are used as weights.
                    for j in range(int((num_col - footer - header) / 2)):
                        quan, err = 0.0, 0.0
                        quan_temp = {}
                        for k in range(num_member[i]):
                            err_ = lines_data[si + k][2 * j + header + 1]
                            err_ = 1.0 / (err_ * err_) if err_ > eps else 0.0
                            err += err_
                            quan += lines_data[si + k][2 * j + header] * err_
                            quan_temp[k] = lines_data[si + k][2 * j + header]
                        if abs(err) > eps:
                            quan /= err
                            err = math.sqrt(1.0 / err)
                        else:
                            quan = quan_temp[0]
                            err = eps
                        context += '\t' + str(format(quan, ".16f")).ljust(wid) + '\t' + str(format(err, ".16f")).ljust(wid)
                    # Write footer information
                    # This line calculates the simple average for the footer columns, as they do not require error propagation.
                    # This is useful for aggregating non-observational metadata.
                    for j in range(num_col - footer, num_col):
                        quan = sum(lines_data[si + k][j] for k in range(num_member[i])) / num_member[i]
                        context += '\t' + str(format(quan, ".16f")).ljust(wid)

                wf.write(context + '\n')

##############################################
wid = 16
eps = 1E-13

header = hf.header  
footer = hf.footer  

indices = hf.indices

seed_index = hf.seed_index - 1

# File names passed as arguments
f_names = sys.argv[1:]

for f_name in f_names:
    compress(f_name, wid, eps, header, footer, indices, seed_index)

##################################################################################
end_time = time.time()
execution_time = end_time - start_time
print(f"Time for compressing data: {execution_time:.4f} seconds")    
