const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="pt-BR">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>UmiTech IoT</title>
  <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0/css/all.min.css">
  <style>
    body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; background-color: #f1f8e9; margin: 0; padding: 20px; color: #33691e; }
    h1 { text-align: center; color: #2e7d32; }
    .cards { display: grid; grid-template-columns: repeat(auto-fit, minmax(200px, 1fr)); gap: 20px; margin-bottom: 30px; }
    .card { background: white; padding: 20px; border-radius: 15px; text-align: center; box-shadow: 0 4px 15px rgba(0,0,0,0.1); transition: 0.3s; }
    .card:hover { transform: translateY(-5px); }
    .card i { font-size: 40px; color: #8bc34a; margin-bottom: 10px; }
    .value { font-size: 24px; font-weight: bold; display: block; }
    table { width: 100%; background: white; border-radius: 10px; overflow: hidden; box-shadow: 0 4px 15px rgba(0,0,0,0.1); border-collapse: collapse; }
    th, td { padding: 12px; text-align: center; border-bottom: 1px solid #eee; }
    th { background-color: #c5e1a5; color: #33691e; }
    .status-on { color: #d32f2f; font-weight: bold; }
    .status-off { color: #388e3c; }
  </style>
</head>
<body>
  <h1>🌿 UmiTech IoT: Controle de Solo</h1>
  <div class="cards">
    <div class="card"><i class="fas fa-thermometer-half"></i><span>Temperatura</span><span id="temp" class="value">--°C</span></div>
    <div class="card"><i class="fas fa-seedling"></i><span>Umidade Solo</span><span id="solo" class="value">--</span></div>
    <div class="card"><i class="fas fa-faucet-drip"></i><span>Bomba d'água</span><span id="bomba" class="value">--</span></div>
  </div>
  <table>
    <thead><tr><th>Registro</th><th>Temp.</th><th>Solo</th><th>Bomba</th></tr></thead>
    <tbody id="tabela-corpo"></tbody>
  </table>
  <script>
    async function carregar() {
      try {
        const r = await fetch('/dados');
        const d = await r.json();
        document.getElementById('temp').innerText = d.atual.temp.toFixed(1) + "°C";
        document.getElementById('solo').innerText = d.atual.solo;
        document.getElementById('bomba').innerText = d.atual.bomba ? "LIGADA" : "DESLIGADA";
        document.getElementById('bomba').className = d.atual.bomba ? "value status-on" : "value status-off";
        
        let html = "";
        d.historico.forEach((h, i) => {
          html += `<tr><td>#${i+1}</td><td>${h.t}°C</td><td>${h.s}</td><td>${h.b}</td></tr>`;
        });
        document.getElementById('tabela-corpo').innerHTML = html;
      } catch(e) { console.log("Erro ao buscar dados"); }
    }
    setInterval(carregar, 3000);
    carregar();
  </script>
</body>
</html>
)rawliteral";