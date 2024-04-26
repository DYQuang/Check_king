function doGet(e) { 
  Logger.log( JSON.stringify(e) );
  var result = 'Ok';
  if (e.parameter == 'undefined') {
    result = 'No Parameters';
  }
  else {
    var sheet_id = '1CqOOzbZGR_zxUcxzjNzLGx8XKJua3M8COxAB1hj9vcA'; 	// Spreadsheet ID
    var sheet = SpreadsheetApp.openById(sheet_id).getActiveSheet();
    var newRow = sheet.getLastRow() + 1;						
    var rowData = [];
    var Curr_Date = new Date();
    rowData[0] = Curr_Date; // Date in column A
    var Curr_Time = Utilities.formatDate(Curr_Date, "Asia/Jakarta", 'HH:mm:ss');
    rowData[1] = Curr_Time; // Time in column B
    for (var param in e.parameter) {
      Logger.log('In for loop, param=' + param);
      var value = stripQuotes(e.parameter[param]);
      Logger.log(param + ':' + e.parameter[param]);
      if(param == 'ID'){
        rowData[2] = value; // data in column C
      }
      if(param == 'solan'){
        rowData[3] = value; // data in column D
      }
    }
    Logger.log(JSON.stringify(rowData));
    var newRange = sheet.getRange(newRow, 1, 1, rowData.length);
    newRange.setValues([rowData]);
  }
  return ContentService.createTextOutput(result);
}
function stripQuotes( value ) {
  return value.replace(/^["']|['"]$/g, "");
}



// function doGet(e) {: Đây là hàm chính xử lý các yêu cầu HTTP GET. Tham số e là một đối tượng sự kiện chứa thông tin về yêu cầu HTTP.

//   Logger.log(JSON.stringify(e));: Ghi log đối tượng sự kiện e dưới dạng chuỗi JSON. Hữu ích để debug và hiểu thông tin được truyền trong yêu cầu.
  
//   var result = 'Ok';: Khởi tạo biến result với giá trị 'Ok'. Biến này sẽ được sử dụng để xác định phản hồi sẽ được gửi trở lại.
  
//   if (e.parameter == 'undefined') { result = 'No Parameters'; }: Kiểm tra xem e.parameter có được định nghĩa hay không. Nếu đúng, đặt result thành 'No Parameters'. Điều này có lẽ là để xử lý các trường hợp yêu cầu không có tham số.
  
//   else {: Nếu có tham số trong yêu cầu:
  
//   var sheet_id = '1CqOOzbZGR_zxUcxzjNzLGx8XKJua3M8COxAB1hj9vcA';: Thiết lập ID của Bảng tính Google. Đây là nơi dữ liệu sẽ được ghi.
  
//   var sheet = SpreadsheetApp.openById(sheet_id).getActiveSheet();: Mở bảng tính đã chỉ định và lấy trang tính hiện tại.
  
//   var newRow = sheet.getLastRow() + 1;: Tìm hàng cuối cùng chứa dữ liệu trong bảng và đặt newRow là hàng tiếp theo.
  
//   var rowData = [];: Khởi tạo một mảng rỗng rowData để lưu trữ dữ liệu sẽ được ghi vào bảng tính.
  
//   var Curr_Date = new Date(); rowData[0] = Curr_Date;: Lấy ngày và giờ hiện tại và lưu ngày vào rowData[0] (cột A).
  
//   var Curr_Time = Utilities.formatDate(Curr_Date, "Asia/Jakarta", 'HH:mm:ss'); rowData[1] = Curr_Time;: Định dạng ngày và giờ hiện tại theo múi giờ đã chỉ định và lưu giờ vào rowData[1] (cột B).
  
//   for (var param in e.parameter) {: Lặp qua từng tham số trong yêu cầu.
  
//   Logger.log('Trong vòng lặp for, param=' + param);: Ghi log tham số hiện tại.
  
//   var value = stripQuotes(e.parameter[param]);: Gọi hàm stripQuotes để loại bỏ dấu ngoặc kép từ giá trị tham số.
  
//   Logger.log(param + ':' + e.parameter[param]);: Ghi log tên và giá trị của tham số.
  
//   if (param == 'ID') { rowData[2] = value; }: Nếu tham số là 'ID', đặt giá trị tương ứng vào rowData[2] (cột C).
  
//   if (param == 'solan') { rowData[3] = value; }: Nếu tham số là 'solan', đặt giá trị tương ứng vào rowData[3] (cột D).
  
//   Logger.log(JSON.stringify(rowData));: Ghi log mảng rowData cuối cùng.
  
//   var newRange = sheet.getRange(newRow, 1, 1, rowData.length);: Xác định phạm vi trong bảng tính nơi dữ liệu mới sẽ được ghi.
  
//   newRange.setValues([rowData]);: Ghi dữ liệu trong rowData vào phạm vi được chỉ định trong bảng tính.
  
//   }: Đóng khối else.
  
//   return ContentService.createTextOutput(result);: Tạo một phản hồi văn bản dựa trên giá trị của result và trả về nó.
  
//   }: Đóng hàm chính doGet.
  
//   function stripQuotes(value) { return value.replace(/^["']|['"]$/g, ""); }: Định nghĩa hàm stripQuotes để loại bỏ dấu ngoặc từ một chuỗi 
