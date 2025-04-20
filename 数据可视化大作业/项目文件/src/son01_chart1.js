import ciyun from "./ciyun_function.js";
import ciyun2 from "./ciyun2_function.js";


const douban_comment_data = "assets/孤星计划/douban_comments.csv"
let douban_data = [];
d3.csv(douban_comment_data).then(data => {
  douban_data = data;
}).catch(error => {
  console.error("加载 CSV 文件时出错：", error);
}); 


let comment_data = [];
comment_data[0] = "assets/孤星计划/孤星计划_comments/comments/2024-12-07.csv";
comment_data[1] = "assets/孤星计划/孤星计划_comments/comments/2024-12-08.csv";
comment_data[2] = "assets/孤星计划/孤星计划_comments/comments/2024-12-09.csv";
comment_data[3] = "assets/孤星计划/孤星计划_comments/comments/2024-12-10.csv";
comment_data[4] = "assets/孤星计划/孤星计划_comments/comments/2024-12-11.csv";
comment_data[5] = "assets/孤星计划/孤星计划_comments/comments/2024-12-12.csv";
comment_data[6] = "assets/孤星计划/孤星计划_comments/comments/2024-12-13.csv";
comment_data[7] = "assets/孤星计划/孤星计划_comments/comments/2024-12-14.csv";
comment_data[8] = "assets/孤星计划/孤星计划_comments/comments/2024-12-15.csv";





let datalist = [];  // 用来存储解析后的数据
let promises = [];  // 用来存储所有的异步请求

// 遍历 comment_data 数组，异步加载每个 CSV 文件
comment_data.forEach(csvFile => {
  let promise = fetch(csvFile)
    .then(response => response.text())  // 获取 CSV 文件的文本内容
    .then(data => {
      // 解析 CSV 数据
      const parsedData = Papa.parse(data, { header: true, skipEmptyLines: true });
      if (parsedData.data.length > 0) {
        datalist.push(parsedData.data);  // 将解析后的数据添加到 datalist
      }
    })
    .catch(error => console.error(`Error fetching CSV file ${csvFile}:`, error));

  // 将每个异步请求添加到 promises 数组
  promises.push(promise);
});

// 等待所有异步请求完成后再处理
Promise.all(promises).then(() => {
  console.log('All CSV files have been processed.');
});
 


export function map_chart(chartId, startDate, endDate, geoJsonPath, csvPathPattern) {
    const colorScale = d3.scaleSequential(d3.interpolateYlOrRd).domain([0, 200]);

    let provinceDataList = [];
    let svg, dateLabel, provinceLabel;
    let selectedProvince = null;


    function initializeMap() {
        const width = 1000, height = 700;
        const projection = d3.geoMercator().center([104, 37]).scale(700).translate([width / 2, height / 2]);
        const path = d3.geoPath().projection(projection);

        svg = d3.select(chartId).append("svg").attr("width", width).attr("height", height);


        dateLabel = svg.append("text").attr("x", width / 2).attr("y", height - 30).style("font-size", "20px")
            .style("text-anchor", "middle").style("fill", "black").text(startDate);

        provinceLabel = svg.append("text").attr("x", width / 2).attr("y", 60)
            .style("font-size", "16px").style("text-anchor", "middle").style("fill", "black").text("");

        d3.json(geoJsonPath).then(function (geoData) {
            svg.selectAll("path")
                .data(geoData.features)
                .enter().append("path")
                .attr("d", path)
                .attr("fill", function (d) {
                    const provinceName = d.properties.name;
                    const dataValue = provinceDataList[0] && provinceDataList[0][provinceName] || 0;
                    return selectedProvince ? (provinceName === selectedProvince ? colorScale(dataValue) : d3.rgb(200, 200, 200)) : colorScale(dataValue);
                })
                .attr("stroke", "#fff")
                .attr("stroke-width", 1)
                .on("click", function (event, d) {
                    const provinceName = d.properties.name;
                    handleProvinceClick(provinceName);
                });
        });

        ciyun(douban_data);



        const slider = d3.select("#slider");
        slider.on("input", function () {
            const sliderValue = +slider.property("value");
            const dataIndex = Math.floor(sliderValue / 10);
            const updatedData = provinceDataList[dataIndex] || {};

            d3.json(geoJsonPath).then(function (geoData) {
                const paths = svg.selectAll("path");
                paths.transition().duration(500)
                    .attr("fill", function (d) {
                        const provinceName = d.properties.name;
                        const dataValue = updatedData[provinceName] || 0;
                        return selectedProvince ? (provinceName === selectedProvince ? colorScale(dataValue) : d3.rgb(200, 200, 200)) : colorScale(dataValue);
                    });
            });

            d3.select("#word-cloud").html("");
            ciyun2(datalist[dataIndex]);

            // 更新日期文本
            const currentDate = generateDateRange(startDate, endDate)[dataIndex];
            dateLabel.text(currentDate);

            // 如果有选中省份，更新该省份的数据
            if (selectedProvince) {
                const dataValue = updatedData[selectedProvince] || 0;
                provinceLabel.text(`省份: ${selectedProvince}, 数据值: ${dataValue}`);
            }
        });

        addColorLegend();
    }

    // 动态加载 CSV 文件
    function loadCSV() {
        const dateRange = generateDateRange(startDate, endDate);

        // 生成所有文件的路径
        const dataFiles = dateRange.map(date => csvPathPattern.replace("{date}", date));
        let dataPromises = dataFiles.map(file => {
            return d3.csv(file).then(function (data) {
                const provinceData = {};
                data.forEach(function (d) {
                    if (d.ip) {
                        provinceData[d.ip] = +d.score;
                    }
                });
                return provinceData;
            });
        });

        Promise.all(dataPromises).then(function (allData) {
            provinceDataList = allData;  // 将所有数据存储到一个列表中
            initializeMap();  // 初始化地图
        });
    }

    // 点击省份时更新图表
    function handleProvinceClick(provinceName) {
        const currentData = provinceDataList[0] || {};
        const dataValue = currentData[provinceName] || 0;

        if (selectedProvince === provinceName) {
            svg.selectAll("path")
                .transition()
                .duration(300)
                .attr("fill", function (d) {
                    const name = d.properties.name;
                    return colorScale(currentData[name] || 0);  // 恢复为正常的颜色
                });
            selectedProvince = null;  // 取消选中
            provinceLabel.text("");  // 清空省份数据文本
        } else {
            svg.selectAll("path")
                .transition()
                .duration(300)
                .attr("fill", function (d) {
                    const name = d.properties.name;
                    return name === provinceName ? colorScale(currentData[name] || 0) : d3.rgb(200, 200, 200);
                });
            selectedProvince = provinceName;
            provinceLabel.text(`省份: ${provinceName}, 数据值: ${dataValue}`);
        }
    }

    function generateDateRange(startDate, endDate) {
        const dates = [];
        let currentDate = new Date(startDate);
        const end = new Date(endDate);

        while (currentDate <= end) {
            const dateStr = currentDate.toISOString().split('T')[0];
            dates.push(dateStr);
            currentDate.setDate(currentDate.getDate() + 1);  // 增加一天
        }

        return dates;
    }

    // 图例
    function addColorLegend() {
        const legendWidth = 300;
        const legendHeight = 30;
        const legendMargin = 20;

        const legend = svg.append("g")
            .attr("transform", `translate(${legendMargin}, ${30})`);

        const legendScale = d3.scaleLinear()
            .domain([0, 200])
            .range([0, legendWidth]);

        legend.append("rect")
            .attr("x", 0)
            .attr("y", 0)
            .attr("width", legendWidth)
            .attr("height", legendHeight)
            .style("fill", "url(#linear-gradient)");

        svg.append("defs").append("linearGradient")
            .attr("id", "linear-gradient")
            .attr("x1", "0%")
            .attr("x2", "100%")
            .attr("y1", "0%")
            .attr("y2", "0%")
            .selectAll("stop")
            .data(d3.range(0, 1.1, 0.1))
            .enter().append("stop")
            .attr("offset", d => `${d * 100}%`)
            .attr("stop-color", d => colorScale(d * 200));

        legend.append("text")
            .attr("x", legendWidth + 20)
            .attr("y", 15)
            .attr("dy", ".35em")
            .style("font-size", "12px")
            .style("text-anchor", "middle")
            .text("0-200");
    }

    loadCSV();
}


export function PieChart(container, data, date) {
    const width = 200;
    const height = 200;
    const radius = Math.min(width, height) / 2;

    const colorScale = d3.scaleOrdinal(d3.schemeCategory10);

    // 格式化日期：确保传递的日期为字符串格式
    const formattedDate = (date instanceof Date) ? date.toLocaleDateString() : date;

    // 创建SVG容器
    const svg = d3.select(container)
        .append("svg")
        .attr("width", width)
        .attr("height", height)
        .append("g")
        .attr("transform", `translate(${width / 2},${height / 2})`);

    // 在饼状图顶部添加日期文本
    svg.append("text")
        .attr("x", 0)
        .attr("y", -height / 2 + 20)  // 控制日期文本位置，稍微往下偏移
        .attr("text-anchor", "middle")
        .style("font-size", "14px")
        .style("font-family", "Arial, sans-serif")  // 设置字体样式
        .text(`Date: ${formattedDate}`);  // 显示传入的日期，确保格式正确

    // 定义饼状图的角度比例尺
    const pie = d3.pie()
        .value(d => d.value);

    // 定义弧形生成器
    const arc = d3.arc()
        .innerRadius(0) // 外弧半径
        .outerRadius(radius); // 内弧半径

    // 数据：这里假设我们传递了一个包含 `good_reviews` 和 `bad_reviews` 的数据
    const pieData = [
        { label: "好评率", value: (data.good_reviews / data.reviews).toFixed(2) },
        { label: "差评率", value: ((data.reviews - data.good_reviews) / data.reviews).toFixed(2) }
    ];

    // 创建饼状图的扇形
    const arcs = svg.selectAll(".arc")
        .data(pie(pieData))
        .enter().append("g")
        .attr("class", "arc");

    arcs.append("path")
        .attr("d", arc)
        .attr("fill", (d, i) => colorScale(i))
        .attr("opacity", 0.5);

    // 添加文字标签
    arcs.append("text")
        .attr("transform", d => `translate(${arc.centroid(d)})`)
        .attr("dy", ".35em")
        .style("text-anchor", "middle")
        .text(d => `${d.data.label}: ${d.data.value}`);
}





export function LineChart(chartId, data) {
    // 计算好评率
    data.forEach(function (d) {
        d.date = new Date(d.date);
        d.good_rate = d.good_reviews / d.reviews;
    });

    const margin = { top: 20, right: 40, bottom: 50, left: 65 };
    const width = 800 - margin.left - margin.right;
    const height = 400 - margin.top - margin.bottom;

    const svg = d3.select(chartId)
        .append("svg")
        .attr("width", width + margin.left + margin.right)
        .attr("height", height + margin.top + margin.bottom)
        .append("g")
        .attr("transform", `translate(${margin.left},${margin.top})`);

    // x, y轴比例尺
    const x = d3.scaleTime()
        .domain(d3.extent(data, d => d.date))
        .range([0, width]);

    const yBoxOffice = d3.scaleLinear()
        .domain([0, d3.max(data, d => d.box_office) * 1.1])
        .range([height, 0]);

    const yReviews = d3.scaleLinear()
        .domain([0, d3.max(data, d => d.reviews) * 1.1])
        .range([height, 0]);

    const colorScale = d3.scaleSequential(d3.interpolateCool)
        .domain([0, 1]);

    // 背景渐变条（修改后的渐变逻辑）
    svg.selectAll("rect.background")
        .data(data)
        .enter().append("rect")
        .attr("x", (d, i) => {
            // 处理左侧边界
            if (i === 0) {
                return 0;
            }
            // 处理非边界日期
            else if (i < data.length - 1) {
                return (x(data[i - 1].date) + x(d.date)) / 2; // 左侧 x 坐标是前一个日期和当前日期的中点
            }
            // 处理右侧边界
            else {
                return (x(data[i].date) + x(data[i - 1].date)) / 2;
            }
        })
        .attr("y", 0)
        .attr("width", (d, i) => {
            // 处理左侧边界
            if (i === 0) {
                return (x(data[i + 1].date) + x(d.date)) / 2;
            }
            // 处理右侧边界
            else if (i === data.length - 1) {
                return (x(data[i].date) - x(data[i - 1].date)) / 2;
            }
            // 处理非边界日期
            else {
                return (x(data[i + 1].date) + x(d.date)) / 2 - (x(d.date) + x(data[i - 1].date)) / 2;
            }
        })
        .attr("height", height)
        .attr("fill", (d, i) => {
            if (i === data.length - 1) {
                // 对于最后一个矩形，我们采用固定的颜色或特殊渐变
                //return "steelblue"; // 例如使用固定的颜色
                //或者使用自定义渐变
                const gradient = svg.append("defs")
                    .append("linearGradient")
                    .attr("id", `gradient-last`)
                    .attr("x1", "0%")
                    .attr("y1", "0%")
                    .attr("x2", "100%")
                    .attr("y2", "0%");
                gradient.append("stop")
                    .attr("offset", "0%")
                    .attr("stop-color", colorScale(d.good_rate))
                    .attr("stop-opacity", 1 - d.good_rate * 0.4);
                gradient.append("stop")
                    .attr("offset", "100%")
                    .attr("stop-color", colorScale(data[i - 1].good_rate))
                    .attr("stop-opacity", 1 - data[i - 1].good_rate * 0.4);
                return `url(#gradient-last)`;
            } else {
                // 渐变颜色只应用到每个矩形的半边
                const gradient = svg.append("defs")
                    .append("linearGradient")
                    .attr("id", `gradient-${i}`)
                    .attr("x1", "0%")
                    .attr("y1", "0%")
                    .attr("x2", "100%")
                    .attr("y2", "0%");

                gradient.append("stop")
                    .attr("offset", "0%")
                    .attr("stop-color", colorScale(d.good_rate))
                    .attr("stop-opacity", 1 - d.good_rate * 0.4);

                gradient.append("stop")
                    .attr("offset", "100%")
                    .attr("stop-color", colorScale(data[i + 1].good_rate))
                    .attr("stop-opacity", 1 - data[i + 1].good_rate * 0.4);

                return `url(#gradient-${i})`;
            }
        })
        .on("mouseover", function (event, d) {
            // 显示饼状图
            const pieChartDiv = d3.select(chartId)
                .append("div")
                .attr("class", "pie-chart-container")
                .style("position", "absolute")
                .style("left", `${event.pageX + 10}px`)
                .style("top", `${event.pageY + 10}px`);

            // 调用饼状图绘制函数
            PieChart(pieChartDiv.node(), d, d.date);
        })
        .on("mouseout", function () {
            // 隐藏饼状图
            d3.select(".pie-chart-container").remove();
        });

    // 绘制票房折线
    const lineBoxOffice = d3.line()
        .x(d => x(d.date))
        .y(d => yBoxOffice(d.box_office));

    svg.append("path")
        .data([data])
        .attr("class", "line")
        .attr("stroke", "blue")
        .attr("d", lineBoxOffice);

    const lineReviews = d3.line()
        .x(d => x(d.date))
        .y(d => yReviews(d.reviews));

    svg.append("path")
        .data([data])
        .attr("class", "line")
        .attr("stroke", "green")
        .attr("d", lineReviews);

    svg.selectAll(".box-office-circle")
        .data(data)
        .enter().append("circle")
        .attr("class", "box-office-circle")
        .attr("cx", d => x(d.date))
        .attr("cy", d => yBoxOffice(d.box_office))
        .attr("r", 5) // 圆形大小
        .style("fill", "blue") // 颜色为蓝色
        .style("stroke", "black") // 圆形边框颜色
        .style("stroke-width", 1); // 圆形边框宽度
    // 绘制评论数的圆形标记
    svg.selectAll(".comment-circle")
        .data(data)
        .enter().append("circle")
        .attr("class", "comment-circle")
        .attr("cx", d => x(d.date))
        .attr("cy", d => yReviews(d.reviews))
        .attr("r", 5)
        .style("fill", "green")
        .style("stroke", "black")
        .style("stroke-width", 1);

    // 添加X轴
    svg.append("g")
        .attr("class", "axis")
        .attr("transform", `translate(0,${height})`)
        .call(d3.axisBottom(x).ticks(d3.timeDay.every(1)));

    // 添加左侧Y轴
    svg.append("g")
        .attr("class", "axis")
        .call(d3.axisLeft(yBoxOffice));

    // 添加右侧Y轴
    svg.append("g")
        .attr("class", "axis")
        .attr("transform", `translate(${width},0)`)
        .call(d3.axisRight(yReviews));

    // 添加图例
    svg.append("text")
        .attr("x", width - 50)
        .attr("y", 20)
        .attr("fill", "blue")
        .text("票房");

    svg.append("text")
        .attr("x", width - 50)
        .attr("y", 40)
        .attr("fill", "green")
        .text("评论数");

    // X轴标签
    svg.append("text")
        .attr("class", "axis-label")
        .attr("x", width / 2)
        .attr("y", height + 40)
        .style("text-anchor", "middle")
        .text("日期");

    // Y轴标签（票房）
    svg.append("text")
        .attr("class", "axis-label")
        .attr("transform", "rotate(-90)")
        .attr("x", -height / 2)
        .attr("y", -50)
        .style("text-anchor", "middle")
        .text("票房（万）");

    // Y轴标签（评论数）
    svg.append("text")
        .attr("class", "axis-label")
        .attr("transform", "rotate(90)")
        .attr("x", height / 2)
        .attr("y", width + 50)
        .style("text-anchor", "middle")
        .text("评论数");
}
