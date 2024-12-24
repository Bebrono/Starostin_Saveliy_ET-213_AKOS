import sys

def main():
    try:
        expression = input().strip()
        result = eval(expression)
        print(result)
    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    main()