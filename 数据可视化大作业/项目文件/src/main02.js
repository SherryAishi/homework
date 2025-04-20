// main.js

import { map_chart } from './son02_chart1.js'; // 从 son02_chart1.js 导入 map_chart 函数
import { LineChart } from './son02_chart1.js';
document.addEventListener("DOMContentLoaded", function() {
    // 设定日期范围和CSV文件路径模式
    const startDate = "2024-11-22";
    const endDate = "2024-12-01";
    const geoJsonPath = "assets/china.json"; // 地图数据路径
    const csvPathPattern = "assets/好东西/好东西_emotion/{date}.csv"; // CSV 文件路径模板
    const csvPath="assets/好东西/好东西票房.csv"
    // 调用封装的 map_chart 函数初始化图表
    map_chart("#chart", startDate, endDate, geoJsonPath, csvPathPattern);

    //chart3
    d3.csv(csvPath).then(data => {
        data.forEach(d => {
            d.date = new Date(d.date);  // 将日期转换为 Date 对象
            d.box_office = +d.box_office;  // 将票房数据转换为数字
            d.reviews = +d.reviews;  // 将评论数转换为数字
            d.good_reviews = +d.good_reviews;  // 将好评数转换为数字
        });

        // 处理完数据后执行折线图的绘制逻辑
        LineChart("#chart1", data);
    }).catch(error => {
        console.error("Error loading CSV data:", error);
    });
});
