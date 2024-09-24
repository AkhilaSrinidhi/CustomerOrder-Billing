import csv
from fpdf import FPDF
import matplotlib.pyplot as plt
from io import BytesIO

# Function to read data from CSV
def read_csv_data(file_name):
    with open(file_name, encoding="utf8") as csv_file:
        return list(csv.reader(csv_file, delimiter=","))

# Function to generate a simple bar chart from the CSV data
def generate_chart(data):
    order_ids = [row[0] for row in data]
    amounts = [float(row[2]) for row in data]
    
    plt.bar(order_ids, amounts, color='blue')
    plt.xlabel('Order ID')
    plt.ylabel('Amount ($)')
    plt.title('Amount per Order')
    
    img_buffer = BytesIO()
    plt.savefig(img_buffer, format='png')
    plt.close()
    img_buffer.seek(0)
    return img_buffer

# Function to generate PDF report
def generate_pdf(data, chart_img_buffer):
    pdf = FPDF()
    pdf.set_font("helvetica", size=14)
    
    # Add a title to the PDF
    pdf.add_page()
    pdf.cell(200, 10, "Customer Orders Report", ln=True, align='C')
    
    # Table header
    pdf.set_font("helvetica", "B", size=12)
    pdf.cell(40, 10, "Order ID", 1)
    pdf.cell(60, 10, "Customer Name", 1)
    pdf.cell(30, 10, "Amount", 1)
    pdf.cell(40, 10, "Status", 1)
    pdf.ln()

    # Table data
    pdf.set_font("helvetica", size=12)
    for row in data:
        pdf.cell(40, 10, row[0], 1)
        pdf.cell(60, 10, row[1], 1)
        pdf.cell(30, 10, row[2], 1)
        pdf.cell(40, 10, row[3], 1)
        pdf.ln()

    # Add a chart image to the PDF
    pdf.add_page()
    pdf.image(chart_img_buffer, x=10, y=10, w=180)
    
    pdf.output("customer_orders_report.pdf")

# Main script execution
data = read_csv_data("customer_orders.csv")
chart_img_buffer = generate_chart(data)
generate_pdf(data, chart_img_buffer)
