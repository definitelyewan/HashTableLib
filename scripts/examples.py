from common.buildHelpers import *
from common.InformUser import *

update = Inform(1)

# mk build
update.message("Creating build dir ..")
replace_folder("build")

# build with cmake
update.message("Running Cmake")
cmake_build(".", "build", ["-DBUILD_HTL_EXAMPLES=ON", "-DBUILD_SHARED_LIBS=ON","-DDEBUG_HTL_SYMBOLS=ON"])

# move to build
update.message("Moving to build dir")
if os.path.exists("build"):
    os.chdir("build")
else:
    quit()

# compile the code
update.message("Compiling libraries")
compile_code("HashTableLib")

# move to tests
update.message("Moving to examples")
if os.path.exists("examples"):
    os.chdir("examples")
else:
    quit()

# compile code
update.message("Compiling tests program")
compile_code("HashTable_charExample")
compile_code("HashTable_structExample")


# call test execs
try:
    if platform == "linux" or platform == "linux2":
        
        if(is_command("valgrind")):
            subprocess.call(["valgrind", "--leak-check=full", "--show-leak-kinds=all", "./HashTable_charExample"])
            subprocess.call(["valgrind", "--leak-check=full", "--show-leak-kinds=all", "./HashTable_structExample"])
        else:
            subprocess.call(["./HashTable_charExample"])
            subprocess.call(["./HashTable_structExample"])
        
    elif platform == "darwin":

        if(is_command("leaks")):
            
            malloc_stack_logging = False

            if(os.environ.get("MallocStackLogging") != "0"):
                malloc_stack_logging = True

            # will get changed back but just in case it needs to be set
            os.environ["MallocStackLogging"] = "1"

            subprocess.call(["leaks", "--atExit", "--list", "--", "./HashTable_charExample"])
            subprocess.call(["leaks", "--atExit", "--list", "--", "./HashTable_structExample"])

            if(malloc_stack_logging == False):
                os.environ["MallocStackLogging"] = "0"
        else:
            subprocess.call(["./HashTable_charExample"])
            subprocess.call(["./HashTable_structExample"])


    elif platform == "win32":
        subprocess.call("HashTable_charExample.exe")
        subprocess.call("HashTable_structExample.exe")
        
except OSError as e:
    if e.errno == errno.ENOENT:
        # program was not found
        print("failed to run a test")
        quit()
    else:
        # program output
        raise

update.message("Done!")
