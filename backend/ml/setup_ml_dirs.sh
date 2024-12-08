#!/bin/bash

# Create main directories
mkdir -p receipt_scanner/{models,preprocessing,ocr,utils}
mkdir -p training
mkdir -p ../data/ml/{models/receipt_scanner,database}

# Create __init__.py files
touch __init__.py
touch receipt_scanner/__init__.py
touch receipt_scanner/models/__init__.py
touch receipt_scanner/preprocessing/__init__.py
touch receipt_scanner/ocr/__init__.py
touch receipt_scanner/utils/__init__.py
touch training/__init__.py

# Create main scanner file
touch receipt_scanner/scanner.py

# Create component files
touch receipt_scanner/models/item_classifier.py
touch receipt_scanner/preprocessing/image_processor.py
touch receipt_scanner/ocr/text_extractor.py
touch receipt_scanner/utils/text_matching.py

# Create training files
touch training/data_preparation.py
touch training/model_trainer.py

# Create database and model placeholder files
touch ../data/ml/models/receipt_scanner/config.json
touch ../data/ml/database/item_database.json

# Add requirements file if it doesn't exist
if [ ! -f ../../requirements.txt ]; then
    echo "Creating requirements.txt..."
    cat > ../../requirements.txt << EOL
easyocr>=1.6.2
torch>=2.0.0
transformers>=4.30.0
opencv-python>=4.8.0
python-Levenshtein>=0.21.0
numpy>=1.24.0
Pillow>=9.5.0
scikit-learn>=1.3.0
fastapi>=0.100.0
python-multipart>=0.0.6
EOL
fi

# Make script executable
chmod +x setup_ml_dirs.sh

echo "Directory structure created successfully!"
echo "Next steps:"
echo "1. Install requirements: pip install -r ../../requirements.txt"
echo "2. Download initial models: python -c 'from transformers import AutoTokenizer, AutoModelForTokenClassification; AutoTokenizer.from_pretrained(\"bert-base-multilingual-cased\"); AutoModelForTokenClassification.from_pretrained(\"bert-base-multilingual-cased\")'"
