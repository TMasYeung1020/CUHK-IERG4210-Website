# <1155203181>
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from wordcloud import WordCloud
import re
import nltk
from nltk import pos_tag
from nltk.corpus import stopwords
from nltk.tokenize import word_tokenize
from nltk.stem import PorterStemmer
import yfinance as yf
import matplotlib.patheffects as path_effects
from datetime import datetime, timedelta
from sklearn.ensemble import RandomForestClassifier
from sklearn.datasets import load_wine
from sklearn.model_selection import learning_curve, train_test_split
from sklearn.metrics import accuracy_score
from sklearn.preprocessing import StandardScaler

nltk.download('stopwords')
nltk.download('punkt_tab')

# Problem 2
def problem_2(input_file, output_file="problem2.png", title="Word Cloud for problem 2", threshold=0):
    seed_number = 5731 # use this as default seed if neccessary
    # step 1: load the text from the file
    text = ""
    with open(input_file, 'r', encoding='utf-8') as file:
        text = file.read()
    
    # write your logic here    
    # step 2: convert the text to frequency dictionary
    # convert all text to lower case, remove punctuations and numbers
    lowercase_text = text.lower() # convert text into lower case
    cleaned_text = re.sub(r'[^a-zA-Z\s]', '', lowercase_text)
    words = cleaned_text.split()# only shows 'a-z and A-Z'
    word_freq = pd.Series(words).value_counts().to_dict()  # Count word frequencies using pandas and convert to dictionary
    
    # step 3: remove all words with frequency <= threshold
    filter_freq_text = {} # Initialize empty dictionary for filtered word frequencies
    for word, freq in word_freq.items():
        if freq > threshold: # Check if frequency is greater than the threshold
            filter_freq_text[word] = freq # Add word and frequency to filtered dictionary if condition is met
    
    # step 4: create a word cloud based on the frequencies
    if not filter_freq_text: # Check if filtered_freq is empty (no words above threshold)
        wordcloud = WordCloud(width=800, height=400, background_color='black').generate('no words')
    else:
        wordcloud = WordCloud(width=800,height=400,background_color='black',random_state=seed_number).generate_from_frequencies(filter_freq_text)
        # Use seed for reproducible random layout

    # plot the word cloud
    plt.figure(figsize=(10,5)) # Create a new figure with 10x5 inch dimensions
    plt.imshow(wordcloud, interpolation='none') # Display word cloud with bilinear interpolation for smooth rendering
    plt.title(title) # Set the plot title to the provided title parameter
    plt.axis('off') # Remove axes for a clean word cloud visualization
    
    plt.savefig(output_file, dpi=600, bbox_inches='tight')
    # do not call plt.show()
    plt.close()  # Close the plot to free memory and prevent display
    

# Problem 3
def problem_3(input_text, output_file="problem3.png", title="Sankey Chart for Problem 3"):
    # write your logic here  
    # step 1: convert all text to lower case, remove punctuations and numbers
    # remove stop words using nltk library
    import plotly.graph_objects as go
    import plotly.io as pio
    from PIL import Image
    import io
    
    lowercase_text = input_text.lower() # convert text into lower case
    nopun_text = re.sub(r'[^\w\s]', '', lowercase_text) # remove punctuation
    nonum_text = re.sub(r'\d+', '', nopun_text) # remove numbers
    tokens = word_tokenize(nonum_text)

    stop_words = set(stopwords.words('english'))
    filtered_tokens = [word for word in tokens if word not in stop_words] # keep all non-stopwords
    
    # step 2: convert tokens to stems
    stemmer = PorterStemmer()
    stemmed = [stemmer.stem(token) for token in filtered_tokens]  # stem everything

    
    # step 3: create Sankey data
    pair_counts = {}  # (original, stem) → count
    for orig, stem in zip(filtered_tokens, stemmed):
        key = (orig, stem)
        if key in pair_counts:
            pair_counts[key] += 1
        else:
            pair_counts[key] = 1

    all_nodes = filtered_tokens + stemmed
    node_index = {node: i for i, node in enumerate(all_nodes)}

    values = []
    sources = filtered_tokens
    targets = stemmed
    dropped_values = []
    remove_count = 0

    for i in range(len(filtered_tokens)):
        dropped_value = 0
        temp = filtered_tokens[i]
        if (temp == filtered_tokens[i-1]):
            dropped_value = 1
        dropped_values.append(dropped_value)

    for j in range(len(dropped_values)):
        if (dropped_values[j] == 1):
            values.pop()
            values.append(6)
            sources.pop(j - remove_count)
            targets.pop(j - remove_count)
            remove_count += 1
        else:
            values.append(3)

    all_nodes2 = sources + targets
    sources_indices = []
    targets_indices = []

    for i in range(len(sources)):
        sources_indices.append(i)
        targets_indices.append(i + len(sources))
    
    # step 4: create Sankey chart
    
    fig = go.Figure(data=[go.Sankey(
    node = dict(
      pad = 15,
      thickness = 20,
      line = dict(color = "black", width = 0.5),
      label = all_nodes2,
      color = "blue"
    ),
    link = dict(
      source = sources_indices,
      target = targets_indices,
      value = values
  ))])

    fig.update_layout(title_text=title, font_size=10)
    fig.show()          # optional – shows the interactive Plotly version
    
    # Force Plotly to use the built-in static renderer (bypasses Kaleido problem)
    pio.renderers.default = "png"

    # Convert Plotly figure → PNG bytes in memory
    png_bytes = pio.to_image(fig, format="png", width=1600, height=900, scale=2)

    # Now plt.savefig() works exactly as your teacher wants
    img = Image.open(io.BytesIO(png_bytes))
    plt.figure(figsize=(16, 9))
    plt.imshow(img)
    plt.axis("off")
    plt.savefig(output_file, dpi=300, bbox_inches="tight", pad_inches=0.1)
    plt.close()

# Problem 4
def problem_4(tickers, start_date, end_date, output_file="problem4.png", title="Stock Price Comparison for Problem 4"):
    # download data
    data = yf.download(tickers, start=start_date, end=end_date)['Close']
    # write your logic here
    # Create the plot
    plt.figure(figsize=(14, 8))
    ax = plt.gca()
    
    # Define 4 different styles — one for each ticker (0=red, 1=blue, 2=green, 3=orange)
    label_styles = [
        {"offset_days": -620, "height": 1.025, "ha": "center",   "va": "center"},  # 1st → far right, high
        {"offset_days":  -300, "height": 0.775, "ha": "left",   "va": "bottom"},  # 2nd → medium right
        {"offset_days": -425, "height": 0.800, "ha": "left",   "va": "top"},     # 3rd → farther right, below
        {"offset_days":  -450, "height": 0.65, "ha": "right",  "va": "top"}      # 4th → almost on point, below
    ]

    for i, ticker in enumerate(data.columns):
        prices = data[ticker].dropna()
        if prices.empty:
            continue

        color = ['red', 'blue', 'green', 'orange'][i]

        ax.plot(prices.index, prices.values, linewidth=2.5, color=color)

        # Pick style based on ticker order
        style = label_styles[i % 4]

        # Choose point near the end (last 20% of data looks best)
        idx = int(len(prices) * 0.85) + i * 5
        idx = min(idx, len(prices) - 1)
        base_date = prices.index[idx]
        base_price = prices.iloc[idx]

        # Apply custom offset and height
        label_date  = base_date + timedelta(days=style["offset_days"])
        label_price = base_price * style["height"]

        text = ax.text(label_date, label_price, ticker,
                       fontsize=10, fontweight='bold', color=color,
                       ha=style["ha"], va=style["va"])

        text.set_path_effects([
            path_effects.Normal()
        ])
    
    # Formatting
    ax.set_title(title, fontsize=16, fontweight='bold', pad=20)
    ax.set_ylabel('Price (USD)', fontsize=12)
    ax.grid(True, alpha=0.3)
    ax.spines['top'].set_visible(False)
    ax.spines['right'].set_visible(False)
    
    plt.tight_layout()
    plt.savefig(output_file, dpi=300, bbox_inches='tight')
    # do not call plt.show()


# Problem 5
def problem_5(data, group_names, output_file="problem5.png", titles=["Subplot Exercise for Problem 5", "Box plot", "Mean values"], shuffle=False):

    df = pd.DataFrame(data).T           # transpose → each array becomes a column
    df.columns = group_names            # assign the correct column names
    
    if shuffle:
        df = df.sample(frac=1, axis=1)
    
    fig, (ax_box, ax_bar) = plt.subplots(1, 2, figsize=(16, 6), width_ratios=[2, 2])

    # Left: Box plot
    colors = ['red', 'orange', 'green']   # 3 colors for your 3 groups
    bp = df.boxplot(ax=ax_box, patch_artist=True, return_type='dict', boxprops=dict(facecolor='lightblue', color='black'),
                    medianprops=dict(color='red'), whiskerprops=dict(color='black'), capprops=dict(color='black'),)
    for patch, color in zip(bp['boxes'], colors):
        patch.set_facecolor(color)

    ax_box.set_ylabel('Values')
    ax_box.set_title(titles[1], fontweight='bold')
            
    # Right: Horizontal bar chart of means
    colors2 = ['red', 'orange', 'green']   # 3 colors for your 3 groups

    means = df.mean()
    bars = ax_bar.barh(means.index, means.values, 
                       color=colors)

    for bar, val in zip(bars, means.values):
        ax_bar.text(val + 1, bar.get_y() + bar.get_height()/2, 
                    f'{val:.2f}', va='center', fontweight='bold')

    ax_bar.set_title(titles[2], fontweight='bold')
    ax_bar.set_xlabel('Values')


    fig.suptitle(titles[0], fontsize=17, y=1.01)
    plt.tight_layout()
    plt.savefig(output_file)
    # do not call plt.show()


# Problem 6
def problem_6(X, y, output_file="problem6.png", title="Learning Curve for Problem 6"):
    seed_number = 5731
    
    # Split and scale (as in your template)
    X_train, X_test, y_train, y_test = train_test_split(
        X, y, test_size=0.2, random_state=seed_number, stratify=y)
    
    scaler = StandardScaler()
    X_train_scaled = scaler.fit_transform(X_train)
    X_test_scaled = scaler.transform(X_test)
    
    # Define the exact training sizes required
    train_sizes = [1, 10, 20, 50, 100, 113]
    
    # Create Random Forest classifier
    rf = RandomForestClassifier(n_estimators=100, random_state=seed_number)
    
    # Generate learning curve data
    train_sizes_abs, train_scores, val_scores = learning_curve(
        estimator=rf,
        X=X_train_scaled,
        y=y_train,
        train_sizes=train_sizes,
        cv=5,                              # 5-fold cross-validation
        scoring='accuracy',
        random_state=seed_number
    )
    
    # Compute mean and std
    train_mean = np.mean(train_scores, axis=1)
    train_std = np.std(train_scores, axis=1)
    val_mean = np.mean(val_scores, axis=1)
    val_std = np.std(val_scores, axis=1)
    
    # Plot
    plt.figure(figsize=(10, 6))
    plt.plot(train_sizes_abs, train_mean, 'o-', color='blue', label='Training score')
    plt.plot(train_sizes_abs, val_mean, 'o-', color='red', label='Cross-validation score')
    
    # Shade the standard deviation area (optional but looks pro)
    plt.fill_between(train_sizes_abs, train_mean - train_std, train_mean + train_std,
                     alpha=0.1, color='blue')
    plt.fill_between(train_sizes_abs, val_mean - val_std, val_mean + val_std,
                     alpha=0.1, color='red')
    
    # Final performance on test set
    rf.fit(X_train_scaled, y_train)
    final_train_acc = accuracy_score(y_train, rf.predict(X_train_scaled))
    final_test_acc = accuracy_score(y_test, rf.predict(X_test_scaled))
    
    # Add final performance text at the end
    plt.text(113, final_train_acc, f'Final Train: {final_train_acc:.3f}', 
             color='blue', fontweight='bold', ha='left', va='center')
    plt.text(113, final_test_acc, f'Final CV: {final_test_acc:.3f}', 
             color='red', fontweight='bold', ha='left', va='center')
    
    # Formatting
    plt.title(title, fontsize=14, fontweight='bold', pad=20)
    plt.xlabel('Training Set Size')
    plt.ylabel('Accuracy Score')
    plt.legend(loc='upper right')
    plt.grid(True, alpha=0.3)
    plt.xlim(-5, 119)
    plt.ylim(0.7, 1.05)
    
    plt.tight_layout()
    plt.savefig(output_file)
    plt.close()
    # do not call plt.show()


if __name__ == "__main__":

    # Testing: Problem 2
    problem_2("problem2.txt", "problem2.png", threshold=3)

    # Testing: Problem 3
    text = ""
    with open("problem3.txt", 'r', encoding='utf-8') as file:
        text = file.read()
    problem_3(text, output_file="problem3.png")

    # Testing: Problem 4
    tickers = ["AAPL", "MSFT", "GOOGL", "AMZN"]
    start_date = "2020-01-01"
    end_date = "2025-01-01"
    problem_4(tickers, start_date, end_date)


    # Testing: Problem 5
    np.random.seed(5731)
    # create sample data for three groups
    data1 = np.random.normal(100, 15, 300)  # Group A
    data2 = np.random.normal(110, 12, 200)  # Group B
    data3 = np.random.normal(95, 18, 400)   # Group C
    # combine data for subplots
    data = [data1, data2, data3]
    group_names = ['Group A', 'Group B', 'Group C']
    problem_5(data, group_names)
    

    # Testing: Problem 6
    wine = load_wine()
    X, y = wine.data, wine.target
    problem_6(X, y)
    
