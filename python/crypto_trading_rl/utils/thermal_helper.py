import subprocess
from time import sleep

from utils.logger import Logger

def get_thermal_pressure_level():
    """
    Ejecuta `sudo powermetrics --samplers thermal -n 1` y devuelve el nivel térmico actual.
    Posibles valores: "Nominal", "Fair", "Heavy", o "Unknown".
    """
    try:
        # Ejecutar el comando
        result = subprocess.run(
            ["sudo", "powermetrics", "--samplers", "thermal", "-n", "1"],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            timeout=10  # Máximo 10 segundos
        )

        if result.returncode != 0:
            print("Error ejecutando powermetrics:", result.stderr.strip())
            return "Unknown"

        output = result.stdout

        # Buscar línea con "Current pressure level"
        for line in output.splitlines():
            if "Current pressure level:" in line:
                level = line.split(":")[-1].strip().capitalize()
                # Validar que sea uno de los valores esperados
                if level in ["Nominal", "Moderate", "Heavy"]:
                    if level == "Heavy":
                        print("🔥 Sobrecalentamiento detectado. Durmiendo 60 segundos...")
                        Logger.debug_print("[✓] Pausing for 60 seconds to avoid overloading the server...")
                        sleep(60)  # Pausa para evitar sobrecargar el servidor

                    return level
                else:
                    return "Unknown"

        return "Unknown"

    except subprocess.TimeoutExpired:
        print("El comando powermetrics excedió el tiempo de espera.")
        return "Unknown"
    except Exception as e:
        print("Error al obtener nivel térmico:", str(e))
        return "Unknown"