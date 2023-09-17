import os
import json

TOOL_DIR = os.path.dirname(__file__)
CACHE_DIR = os.path.join(TOOL_DIR, ".cache")

# Cache data into a json file
def cache_data(cache_name, data):
    if not os.path.isdir(CACHE_DIR):
        os.mkdir(CACHE_DIR)

    with open(os.path.join(CACHE_DIR, f"{cache_name}.cache"), 'w') as f:
        json.dump(data, f)

# Recover cached data from a json file
def get_data_from_cache(cache_name):
    file_path = os.path.join(CACHE_DIR, f"{cache_name}.cache")
    print(f"[INFO] Looking for {file_path}")
    if not os.path.isfile(file_path):
        print(f"[INFO] Not found in cache: \"{cache_name}\"")
        return None

    with open(file_path, "r") as f:
        data = json.load(f)
        return data
