// src/components/views/ChatView.tsx

import React, { useState } from 'react';
import type { Message } from '../../../App';
import { FaPaperPlane, FaCheck, FaExclamationCircle } from 'react-icons/fa';
import './ChatView.css'; // Importando o CSS para estilização

interface ChatViewProps {
  messages: Message[];
  onSendMessage: (message: string) => void;
}

const ChatView: React.FC<ChatViewProps> = ({ messages, onSendMessage }) => {
  const [inputValue, setInputValue] = useState('');

  const handleSend = () => {
    if (inputValue.trim()) {
      onSendMessage(inputValue);
      setInputValue('');
    }
  };

  // Função para renderizar o indicador de status da mensagem
  const renderMessageStatus = (status: Message['status']) => {
    switch (status) {
      case 'sending':
        return <div className="spinner" title="Enviando..."></div>;
      case 'sent':
        // Usamos um ícone de check para 'enviado'
        return <FaCheck className="status-icon sent" title="Enviado" />;
      case 'failed':
        return (
          <div className="failed-container">
            <FaExclamationCircle className="status-icon failed" title="Falha ao enviar" />
            {/* Adicionamos um botão para o usuário poder tentar de novo */}
            <button className="resend-btn">Reenviar</button>
          </div>
        );
      default:
        return null;
    }
  };

  return (
    <div className="chat-view">
      <div className="chat-info-bar">
        <span className='chat-info-item'>Conectado na porta: <strong>8080</strong></span>
        <span className='chat-info-item'>Nome de usuário conectado: <strong>Anonimo</strong></span>
        <span className='chat-info-item'>localhost:8080</span>
      </div>
      <div className="messages-list">
        {messages.map((msg) => (
          <div
            key={msg.id}
            className={`message-row ${msg.sender}`}
            style={{
              display: 'flex',
              flexDirection: 'row',
              justifyContent: msg.sender === 'me' ? 'flex-end' : 'flex-start',
              alignItems: 'flex-end',
              gap: '8px',
            }}
          >
            {msg.sender === 'other' && (
              <div style={{ width: 32 }} />
            )}
            <div className={`message-bubble ${msg.sender}`}>{msg.text}</div>
            {msg.sender === 'me' && (
              <div className="status-container" style={{ minWidth: 32, display: 'flex', alignItems: 'center', justifyContent: 'flex-end' }}>
                {renderMessageStatus(msg.status)}
              </div>
            )}
          </div>
        ))}
      </div>
      <div className="message-input-area" style={{ position: 'sticky', bottom: 0, background: 'var(--main-bg)' }}>
        <input
          type="text"
          value={inputValue}
          onChange={(e) => setInputValue(e.target.value)}
          onKeyDown={(e) => e.key === 'Enter' && handleSend()}
          placeholder="Digite sua mensagem..."
        />
        <button onClick={handleSend}><FaPaperPlane /></button>
      </div>
    </div>
  );
};

export default ChatView;