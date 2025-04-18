#### easy to process data.
mkdir -p cmp
mkdir -p raw

cd script

#### format the raw data in jobs or test
python3 1_format_jobs.py

# #### collect all the formatted data
python3 2_collect_data.py

# #### decompose the data in terms of specified parameters
python3 3_extract_data.py  ../raw/raw.dat

# #### compress the data in terms of different seeds
python3 4_compress_data.py ../raw/raw_*
