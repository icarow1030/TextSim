// src/components/views/Log/LogView.tsx
import React, { useRef, useEffect } from 'react';
import type { LogEntry } from '../../../App';
import './LogView.css';
import pdfIcon from './pdf_logo.png'; // 1. Importe a imagem do ícone do PDF

interface LogViewProps {
  logs: LogEntry[];
  deleteAllLogs: () => void;
  // Futuramente, você pode adicionar uma prop para a função de gerar PDF
}

const LogView: React.FC<LogViewProps> = ({ logs, deleteAllLogs }) => {
  const logContainerRef = useRef<HTMLDivElement>(null);

  useEffect(() => {
    if (logContainerRef.current) {
      logContainerRef.current.scrollTop = logContainerRef.current.scrollHeight;
    }
  }, [logs]);

  // 2. Função placeholder para a geração do PDF
  const handleGeneratePdf = () => {
    alert('A lógica para gerar o relatório em PDF será implementada aqui!');
    // Aqui você chamaria a função com jsPDF que discutimos anteriormente
  };

  return (
    <div className="log-view">
      <div className="log-header">
        <h2>Logs do Sistema</h2>
        {/* 3. Container para os botões de ação */}
        <div className="header-actions">
          <button onClick={handleGeneratePdf} className="action-btn pdf-btn">
            <img src={pdfIcon} alt="Gerar PDF" className="btn-icon" />
            <span>Gerar PDF Educativo</span>
          </button>
          <button onClick={deleteAllLogs} className="action-btn clear-log-btn">
            Limpar Logs
          </button>
        </div>
      </div>
      <div className="log-container" ref={logContainerRef}>
        {logs.map(log => (
          <div key={log.id} className="log-entry">
            <span className="log-timestamp">{log.timestamp}</span>
            <span
              className={`log-level level-${log.level.toLowerCase()}`}
              style={log.color ? { color: log.color } : {}}
            >
              {log.level}
            </span>
            <span className="log-message">{log.message}</span>
          </div>
        ))}
      </div>
    </div>
  );
};

export default LogView;