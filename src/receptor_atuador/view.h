const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="pt-BR">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>UmiTech IoT</title>
  <style>
    * { margin: 0; padding: 0; box-sizing: border-box; font-family: 'Segoe UI', sans-serif; }
    body { background-color: #f1f8e9; color: #33691e; padding: 20px; display: flex; flex-direction: column; align-items: center; min-height: 100vh; }
    header { display: flex; align-items: center; gap: 10px; margin-bottom: 25px; }
    header h1 { color: #2e7d32; font-size: 1.6rem; }
    .cards-container { display: grid; grid-template-columns: repeat(auto-fit, minmax(180px, 1fr)); gap: 15px; width: 100%; max-width: 900px; margin-bottom: 25px; }
    .card { background-color: white; padding: 20px; border-radius: 12px; box-shadow: 0 2px 10px rgba(0,0,0,0.08); text-align: center; }
    
    /* Estilos do Botão Interativo */
    .btn-card { cursor: pointer; transition: transform 0.2s, box-shadow 0.2s; user-select: none; }
    .btn-card:active { transform: scale(0.96); }
    .rotina-ativa { background-color: #e8f5e9; border: 2px solid #a5d6a7; }
    .rotina-ativa .value { color: #2e7d32; }
    .rotina-bloqueada { background-color: #ffebee; border: 2px solid #ef9a9a; }
    .rotina-bloqueada .value { color: #c62828; }
    
    .label { font-size: 0.75rem; text-transform: uppercase; color: #7cb342; font-weight: bold; display: block; margin-bottom: 10px; }
    .value { font-size: 1.5rem; font-weight: bold; }
    .bomba-ligada .value { color: #1b5e20 !important; } 
    .bomba-desligada .value { color: #b71c1c !important; }
    .table-container { width: 100%; max-width: 900px; background: white; border-radius: 12px; overflow: hidden; box-shadow: 0 2px 10px rgba(0,0,0,0.08); }
    table { width: 100%; border-collapse: collapse; font-size: 0.85rem; }
    thead { background-color: #c5e1a5; }
    th, td { padding: 12px; text-align: center; }
    tbody tr:nth-child(even) { background-color: #f9fbe7; }
    .tabela-ligada { color: #1b5e20; font-weight: bold; }
    .tabela-desligada { color: #b71c1c; font-weight: bold; }
    @keyframes flash { 0% { background-color: #dcedc8; } 100% { background-color: transparent; } }
    .new-row { animation: flash 1.5s ease-out; }
  </style>
</head>
<body>
  <header><h1>🌿 UmiTech IoT: Controle de Solo 🌿</h1></header>
  <div class="cards-container">
    <div class="card"><span class="label">Temperatura Ar</span><span id="temp" class="value" style="color:#1b5e20">--</span></div>
    <div class="card"><span class="label">Umidade Solo</span><span id="solo" class="value" style="color:#1b5e20">--</span></div>
    <div id="status-card" class="card"><span class="label">Bomba d'água</span><span id="bomba" class="value">--</span></div>
    <div id="rotina-card" class="card btn-card" onclick="AlternarRotina()"><span class="label">⚙️ Modo Automação</span><span id="rotina-status" class="value">--</span></div>
  </div>
  <div class="table-container">
    <table>
      <thead><tr><th>Data / Hora</th><th>Temp.</th><th>Umidade (%)</th><th>Status Bomba</th></tr></thead>
      <tbody id="tabela-corpo"></tbody>
    </table>
  </div>
  <script>
    let historicoLocal = [];
    let falhas = 0;

    // Função que chama o ESP32 ao clicar no botão
    function AlternarRotina() {
      fetch('/toggleRotina')
      .then(response => response.text())
      .then(() => carregar()) // Atualiza a tela imediatamente após clicar
      .catch(err => console.log("Erro ao alternar modo: " + err.message));
    }

    function carregar() {
      // Cria um controlador para cancelar a requisição se demorar mais de 4 segundos
      const controller = new AbortController();
      const timeoutId = setTimeout(() => controller.abort(), 4000);

      fetch('/dados', { signal: controller.signal })
      .then(response => {
        clearTimeout(timeoutId);
        if (!response.ok) throw new Error('Erro na resposta');
        return response.json();
      })
      .then(d => {
        falhas = 0;
        const agora = new Date().toLocaleString('pt-BR', {
          day: '2-digit', month: '2-digit', year: '2-digit',
          hour: '2-digit', minute: '2-digit', second: '2-digit'
        });
        
        // Atualiza Cards normais
        document.getElementById('temp').innerText = d.atual.temp > 0 ? d.atual.temp.toFixed(1) + "°C" : "ERRO";
        document.getElementById('solo').innerText = (typeof d.atual.umid === "number") ? (d.atual.umid.toFixed(0) + "%") : "--";
        
        const bCard = document.getElementById('status-card');
        const bTxt = document.getElementById('bomba');
        const statusBomba = d.atual.bomba;
        
        bTxt.innerText = statusBomba ? "LIGADA" : "DESLIGADA";
        bCard.className = statusBomba ? "card bomba-ligada" : "card bomba-desligada";

        // ATUALIZA O CARD DE BOTÃO DA ROTINA
        const rCard = document.getElementById('rotina-card');
        const rTxt = document.getElementById('rotina-status');
        if (d.atual.rotina) {
          rTxt.innerText = "DISPONÍVEL";
          rCard.className = "card btn-card rotina-ativa";
        } else {
          rTxt.innerText = "INOPERANTE";
          rCard.className = "card btn-card rotina-bloqueada";
        }

        // Atualiza Tabela
        const novo = { h: agora, t: d.atual.temp, s: d.atual.umid, b: statusBomba };
        
        if(historicoLocal.length === 0 || historicoLocal[0].s !== novo.s || historicoLocal[0].b !== novo.b) {
          historicoLocal.unshift(novo);
          if(historicoLocal.length > 8) historicoLocal.pop();
          render(true);
        }
      })
      .catch(error => {
        falhas++;
        console.log("Tentativa de reconexão (" + falhas + "): " + error.message);
      });
    }

    function render(efeito) {
      let h = "";
      historicoLocal.forEach((l, i) => {
        const classeBomba = l.b ? 'tabela-ligada' : 'tabela-desligada';
        const textoBomba = l.b ? 'LIGADA' : 'DESLIGADA';
        const tempTexto = l.t > 0 ? l.t.toFixed(1) + '°C' : '---';
        const umidTexto = (typeof l.s === "number") ? (l.s.toFixed(0) + '%') : '---';
        
        h += `<tr ${efeito && i==0 ? "class='new-row'":""}>
                <td>${l.h}</td>
                <td>${tempTexto}</td>
                <td>${umidTexto}</td>
                <td class="${classeBomba}">${textoBomba}</td>
              </tr>`;
      });
      document.getElementById('tabela-corpo').innerHTML = h;
    }

    // Intervalo de 5 segundos para não sobrecarregar o rádio do ESP32
    setInterval(carregar, 5000); 
    carregar();
  </script>
</body>
</html>
)rawliteral";