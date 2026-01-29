from flask import Flask, request, render_template
import subprocess, os, random

app = Flask(__name__)

# Path to your compiled C backend executable
BACKEND = os.path.join(os.path.dirname(__file__), "backend", "backend.exe")

# Track used IDs and names
used_ids = set()
used_names = set()

def run_c(input_text):
    """Run the C backend with a given command string."""
    try:
        p = subprocess.Popen(
            [BACKEND],
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
        )
        output, error = p.communicate(input_text)
        if p.returncode != 0:
            return f"Error: {error}"
        return output
    except Exception as e:
        return f"Exception occurred: {str(e)}"

def generate_unique_id():
    """Generate a random unique event ID."""
    while True:
        new_id = random.randint(1000, 9999)  # 4-digit ID
        if new_id not in used_ids:
            used_ids.add(new_id)
            return new_id

@app.route("/")
def home():
    return render_template("index.html", used_names=list(used_names))

@app.route("/insert", methods=["POST"])
def insert():
    data = request.form
    event_id = generate_unique_id()
    name = data['name']
    used_names.add(name)

    cmd = f"INSERT {event_id} {name} {data['loc']} {data['date']} {data['seats']}\n"
    result = run_c(cmd)
    return render_template("index.html", output=result, used_names=list(used_names))

@app.route("/search", methods=["POST"])
def search():
    result = run_c(f"SEARCH {request.form['id']}\n")
    return render_template("index.html", output=result, used_names=list(used_names))

@app.route("/display", methods=["GET"])
def display():
    result = run_c("DISPLAY_F\n")
    return render_template("index.html", output=result, used_names=list(used_names))

# Extra informational pages
@app.route("/application")
def application():
    return render_template("application.html")

@app.route("/unique")
def unique():
    return render_template("unique.html")

@app.route("/problem")
def problem():
    return render_template("problem.html")

if __name__ == "__main__":
    app.run(debug=True)