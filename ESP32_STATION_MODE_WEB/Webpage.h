const char MAIN_page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>ESP32 Ticket</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      background: #f4f6f9;
      display: flex;
      justify-content: center;
      align-items: center;
      height: 100vh;
    }
    .card {
      background: white;
      padding: 20px 30px;
      border-radius: 12px;
      box-shadow: 0 4px 15px rgba(0,0,0,0.2);
      text-align: center;
      width: 320px;
    }
    h2 { margin-bottom: 10px; }
    .btn {
      margin-top: 15px;
      display: inline-block;
      padding: 10px 20px;
      background: #0078d7;
      color: white;
      text-decoration: none;
      border-radius: 8px;
      transition: background 0.3s;
    }
    .btn:hover {
      background: #005ea3;
    }
    input[type=text] {
      padding: 8px;
      width: 70%;
      border: 1px solid #ccc;
      border-radius: 6px;
      margin-bottom: 10px;
    }
    form {
      margin-top: 15px;
    }
  </style>
</head>
<body>
  <div class="card">
    <h2>ESP32 Ticket</h2>
    <p><strong>Name:</strong> %NAME%</p>
    <p><strong>Time:</strong> %TIME%</p>
    <p><strong>ID:</strong> %ID%</p>

    <form action="/generate" method="get">
      <input type="text" name="name" placeholder="Enter name" required>
      <br>
      <button class="btn" type="submit">Generate Ticket</button>
    </form>
    <a href="/download" class="btn">Download Ticket</a>
  </div>
</body>
</html>
)rawliteral";
