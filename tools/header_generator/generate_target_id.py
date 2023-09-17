from HFileGenerator import *
import cache_manager

from selenium import webdriver
from selenium.webdriver import Chrome
from selenium.webdriver.chrome.service import Service
from selenium.webdriver.common.by import By
from webdriver_manager.chrome import ChromeDriverManager

TARGET_ID_CACHE = 'target_id'

MAX_ID = 3703

def init_web_driver():
    # start by defining the options
    options = webdriver.ChromeOptions()
    # options.headless = True # it's more scalable to work in headless mode
    options.add_argument('--headless')
    # normally, selenium waits for all resources to download
    # we don't need it as the page also populated with the running javascript code.
    options.page_load_strategy = 'none'
    # this returns the path web driver downloaded
    chrome_path = ChromeDriverManager().install()
    chrome_service = Service(chrome_path)
    # pass the defined options and service objects to initialize the web driver
    driver = Chrome(options=options, service=chrome_service)
    driver.implicitly_wait(5)
    return driver

# Scrape the fiesta-wiki website
# XXX: Find the game file referencing this info
def scrape_wiki_target_names():
    driver = init_web_driver()

    target_names = {}
    for current_id in range(0, MAX_ID + 1):
        url = f"https://fiesta-wiki.com/mobs/{current_id}"
        driver.get(url)
        try:
            mob_name = driver.find_element(By.CSS_SELECTOR, "strong").text
            if mob_name == '' or '?' in mob_name:
                print(f"[WARN] Weird mob name: \"{mob_name}\"")
                mob_name = f"Unknown #{str(current_id)}"
        except:
            print(f"[WARN] Unable to locate entity name for #{current_id}")
            mob_name = f"Unknown #{str(current_id)}"
        target_names[current_id] = mob_name
        print(f"{current_id} / {MAX_ID} : {mob_name}")

    driver.quit()
    return target_names

def get_target_names(use_cache):
    target_names = None

    if use_cache:
        target_names = cache_manager.get_data_from_cache(TARGET_ID_CACHE)
        # Convert keys to INT
        target_names = { int(k): v for k, v in target_names.items() }

    if target_names is None:
        if use_cache:
            print("[INFO] Target names by ID not found in cache, scraping wiki...")
        target_names = scrape_wiki_target_names()
        cache_manager.cache_data(TARGET_ID_CACHE, target_names)
    return target_names

def generate_target_id_enum(target_names):
    # Find the biggest ID in the dict
    max_id = max(target_names.keys())
    # Generate a list of TARGET_ID_NAME
    target_id_enum_with_dups = [ x for x in range(0, max_id + 1)]
    for index in range(0, max_id + 1):
        name = target_names[index]
        target_id_enum_with_dups[index] = f"TARGET_ID_{const_format(name)}"

    # The same TARGET_ID_X MIGHT appear multiple times
    # We need to fix that in order to have valid C code
    id_counts = dict.fromkeys(target_id_enum_with_dups, 0)
    target_id_enum = []
    for id in target_id_enum_with_dups:
        id_counts[id] += 1
        if id_counts[id] > 1:
            id = f"{id}_{id_counts[id] - 1}"
        target_id_enum.append(id)

    # Append _0 to the first occurrence of duplicated IDs
    duplicates = dict(filter(lambda x: x[1] > 1, id_counts.items()))
    for id in duplicates.keys():
        index = target_id_enum.index(id)
        target_id_enum[index] = f"{id}_0"
    return target_id_enum

def generate_target_id_file(include_path, use_cache):
    target_names = get_target_names(use_cache)
    target_id_enum = generate_target_id_enum(target_names)
    target_id_enum.append("TARGET_ID_MAX")

    id_header = HFileGenerator(include_path, "fopd_target_id.h")
    id_header.add_enum(target_id_enum)
    id_header.add_array(target_names, "char *", "target_names")
    id_header.generate_file()
