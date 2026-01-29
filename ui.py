import subprocess
import os

# absolute path to backend.exe
backend_path = os.path.join(os.path.dirname(__file__), "backend.exe")

print("Looking for:", backend_path)

process = subprocess.Popen(
    [backend_path],
    stdin=subprocess.PIPE,
    stdout=subprocess.PIPE,
    text=True
)

output = process.communicate("5 7\n")[0]
print("Output from C:", output)
