from PIL import Image

# Đường dẫn tệp hình ảnh
input_path = r"C:\Users\datzi\Downloads\R (1).gif" #Link part 
output_path = r"C:\Users\datzi\Downloads\251PAC_resized.png" #out

# Mở hình ảnh
image = Image.open(input_path)

# Chuyển đổi hình ảnh sang thang độ xám (nếu cần) và resize thành 128x64
resized_image = image.convert('L').resize((128, 64), Image.Resampling.LANCZOS)  # LANCZOS giữ độ sắc nét cao

# Lưu hình ảnh đã chỉnh sửa
resized_image.save(output_path)

print(f"Hình ảnh đã được chuyển đổi và lưu tại: {output_path}")
