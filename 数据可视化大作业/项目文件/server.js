const express = require('express');
const path = require('path');
const http = require('http');

// 创建 express 实例
const app = express();

// 设置静态文件目录
app.use(express.static(path.join(__dirname, 'src')));

// 尝试启动服务器的函数
function startServer(port) {
  app.listen(port, () => {
    console.log(`Server running at http://localhost:${port}`);
  }).on('error', (err) => {
    if (err.code === 'EADDRINUSE') {
      // 端口被占用，尝试下一个端口
      console.log(`Port ${port} is already in use, trying another port...`);
      startServer(port + 1);  // 尝试下一个端口
    } else {
      // 其他错误
      console.error('Error starting server:', err);
    }
  });
}

// 默认端口 3001
const port = 3001;
startServer(port);
