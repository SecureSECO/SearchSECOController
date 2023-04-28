import json

# Read the input JSON file
with open('/controller/licenseMatrix/matrixseqexpl.json', 'r') as f:
    data = json.load(f)

# Convert the JSON data
output_data = {
    "timeformat": data["timeformat"],
    "timestamp": data["timestamp"]
}

for license_entry in data["licenses"]:
    license_name = license_entry["name"]
    compatibilities = {}
    
    for compatibility in license_entry["compatibilities"]:
        compat_name = compatibility["name"]
        compat_explanation = compatibility["explanation"]
        compatibilities[compat_name] = compat_explanation

    output_data[license_name] = compatibilities

# Write the converted data to the output JSON file
with open('/controller/licenseMatrix/matrixExpl.json', 'w') as f:
    json.dump(output_data, f, indent=2)
